//
//  verifier.cpp
//  Bits
//
//  Created by Burak on 17.12.2022.
//

#include "verifier.h"
#include "../parser/parser.h"

uint32_t VerifierSync::syncHeight;
utreexo::Pollard VerifierSync::forestState(0);
std::vector<UTXO> VerifierSync::coinbaseUTXOs;

//https://github.com/bitcoin/bitcoin/blob/f3bc1a72825fe2b51f4bc20e004cef464f05b965/src/validation.cpp#L1462
CAmount GetBlockSubsidy(int nHeight)
{
    int halvings = nHeight / 210000;
    // Force block reward to zero when right shift is undefined.
    if (halvings >= 64)
        return 0;

    CAmount nSubsidy = CAmount(5000000000);
    // Subsidy is cut in half every 210,000 blocks which will occur approximately every 4 years.
    nSubsidy >>= halvings;
    return nSubsidy;
}

bool Verifier::verify(valtype rawBlock, valtype rawSpendings, std::vector<uint8_t> proofBytes) {
    
    bool ret = true;
    
    //reorg?
    if(VerifierSync::syncHeight >= HeaderSync::syncHeight)
        ret = false;
    
    // 1. Craft block template
    Block vb = Block::submitNewBlock(rawBlock);
        
    // 2. Import Partial UTXOs
    Exract prevouts;
    if(!prevouts.importUTXOsPartial(&rawSpendings))
        ret = false;
        
    for (int i = 0; i < vb.transactions.size(); i++) {
        
        // 3. Push tx id of each transaction for later merkle root check
        txIDs.push_back(vb.transactions[i].txid);
        
        // 4. BIP-30 check except the two historic blocks at heights #91842 and #91880
        if(i == 0 && ((VerifierSync::syncHeight + 1) != 91842) && ((VerifierSync::syncHeight + 1) != 91880))
            if(VerifierSync::returnCoinbaseUTXOIndex(vb.transactions[0].txid) != 0)
                ret = false;
        
        // 5. Inputs
        for(int in = 0; in < vb.transactions[i].inputs.size(); in++) {
            
            if(i == 0) {
                // COINBASE INPUT
                if ((vb.transactions[0].inputs.size() != 1) ||
                    (vb.transactions[0].inputs[0].scriptSig.size() > 100) ||
                    (vb.transactions[0].inputs[0].scriptSig.size() < 2))
                    ret = false;
                
                // Block subsidty
                inputSats += GetBlockSubsidy(getThisHeight());
            }
            else {
                //NON-COINBASE INPUT
                
                // Is this input from this block?
                uint32_t index = Verifier::returnCollectionIndex(vb.transactions[i].inputs[in].prevOutHash, vb.transactions[i].inputs[in].voutIndex);
                
                if(index != 0) {
                    // Outer UTXO
                    
                    uint32_t cbIndex = VerifierSync::returnCoinbaseUTXOIndex(vb.transactions[i].inputs[in].prevOutHash, vb.transactions[i].inputs[in].voutIndex);
                    if(cbIndex != 0) {
                        //Coinbase maturity check
                        if((getThisHeight() - VerifierSync::coinbaseUTXOs[cbIndex].height) < 100)
                            ret = false;
                        //Push cb spent index
                        cbSpentIndexes.push_back(cbIndex);
                    }
                    
                    UTXO thisUTXO = UTXO(prevouts.partialUTXOs[prevouts.elapsed].height,
                                         vb.transactions[i].inputs[in].prevOutHash,
                                         vb.transactions[i].inputs[in].voutIndex,
                                         prevouts.partialUTXOs[prevouts.elapsed].value,
                                         prevouts.partialUTXOs[prevouts.elapsed].scriptPubkey);
                    prevouts.elapsed++;
                    spentHashes.push_back(thisUTXO.returnLeafHash());
                    
                    //thisUTXO Evalscript()..
                }
                else {
                    // Same-block-UTXO
                    UTXO thisUTXO = *OutCollection[index];
                    spentHashes.push_back(thisUTXO.returnLeafHash());
                    
                    //thisUTXO Evalscript()..
                    
                    OutCollection.erase(OutCollection.begin() + index);
                    delete OutCollection[index];
                }
            }
        }
        
        // 6. Outputs
        for(int out = 0; out < vb.transactions[i].outputs.size(); out++) {
            OutCollection.push_back(new UTXO(getThisHeight(),
                                     vb.transactions[i].txid,
                                     out,
                                     vb.transactions[i].outputs[out].amount,
                                     vb.transactions[i].outputs[out].scriptPubkey));
            outputSats += vb.transactions[i].outputs[out].amount;
        }
    }
    
    // 7. Craft batchproof
    batchProof.Serialize(proofBytes);
        
    // 8. Verify forest state
    if(!VerifierSync::forestState.Verify(batchProof, spentHashes))
        ret= false;
        
    // 9. Inflation check
    if(inputSats != outputSats)
        ret = false;
    
    // 10. Merkle root match
    if(returnMerkleRoot(txIDs) != Header::getHeaderMerkeRoot(VerifierSync::syncHeight + 1))
        ret = false;
            
    if(!ret)
        return ret;
        
    // AFTER VALIDATIONS
        
    // 11. Free coinbase spents from memory
    for (int i = 0; i < cbSpentIndexes.size(); i++) {
        VerifierSync::coinbaseUTXOs.erase(VerifierSync::coinbaseUTXOs.begin() + cbSpentIndexes[i]);
    }
    
    // 12. Free previos header from memory
    if ((((Header*)Header::headerAddresses[0])->height) < (VerifierSync::syncHeight + 1)) {
        delete (Header*)Header::headerAddresses[0];
        Header::headerAddresses.erase(Header::headerAddresses.begin());
    }
        
    // 13. Update forest state
    std::vector<Leaf> newLeaves;
    for (int i = 0; i < OutCollection.size(); i++) {
        newLeaves.emplace_back(OutCollection[i]->returnLeafHash(), false);
    }
    VerifierSync::forestState.Modify(newLeaves, batchProof.GetTargets());
    
    // 14. Increment sync height
    VerifierSync::syncHeight++;
    
    return ret;
    }

VerifierSync::VerifierSync() {
    this->syncHeight = 0;
    this->forestState = utreexo::Pollard(0);
}

VerifierSync::VerifierSync(uint32_t startHeight, uint64_t numLeaves, uint64_t numRoots, std::array<unsigned char, 1024>roots, uint32_t numCoinbaseUTXOs, std::array<unsigned char, 5000000> InCBUTXOs) {

    std::vector<Hash> hashes;
    int elapsed = 0;
    
    for (int i = 0; i < numRoots; i++) {
        Hash hash;
        for (int k = 0; k < 32; k++) {
            hash[k] =(uint8_t)roots[elapsed];
        }
        hashes.push_back(hash);
    }
    
    valtype rawImportCBUTXOs;
    for (int i = 0; i < numCoinbaseUTXOs; i++) {
        rawImportCBUTXOs.push_back(InCBUTXOs[i]);
    }
    
    Exract cbUTXOsImport;
    cbUTXOsImport.importUTXOs(rawImportCBUTXOs);
    
    for (int i = 0; i < cbUTXOsImport.UTXOs.size(); i++) {
        this->coinbaseUTXOs.push_back(cbUTXOsImport.UTXOs[i]);
    }

    this->forestState = utreexo::Pollard(hashes, numLeaves);
    this->syncHeight = startHeight;
}

std::array<unsigned char, 1024> VerifierSync::getRoots() {
    std::array<unsigned char, 1024> ret;
    
    std::vector<Hash> roots;
    forestState.Roots(roots);

    int elapsed = -1;
    for(int i = 0; i < roots.size(); i++) {
        for(int l = 0; l < 32; l++) {
            ret[++elapsed] = (unsigned char)(roots[i])[l];
        }
    }

    return ret;
}

uint64_t VerifierSync::getNumRoots() {
    std::vector<Hash> roots;
    forestState.Roots(roots);
    return roots.size();
}

uint64_t VerifierSync::getNumLeaves() {
    return forestState.NumLeaves();
}

uint32_t VerifierSync::getSyncHeight() {
    return syncHeight;
}

std::array<unsigned char, 5000000> VerifierSync::getCoinbaseUTXOs() {
    Exract p;
    valtype ret = p.exportUTXOs(VerifierSync::coinbaseUTXOs);
    std::array<unsigned char, 5000000> ar;
    
    int elapsed = -1;
    
    for (int i = 0; i < ret.size(); i++) {
        ar[++elapsed] = ret[i];
    }
    
    return ar;
}

uint32_t VerifierSync::getCoinbaseUTXOsSize() {
    Exract p;
    valtype ret = p.exportUTXOs(VerifierSync::coinbaseUTXOs);
    return (uint32_t)ret.size();
}

uint32_t VerifierSync::returnCoinbaseUTXOIndex(valtype prevHash, uint32_t vout) {
    uint32_t ret = 0;
    
    for (int i = 0; i < coinbaseUTXOs.size(); i++) {
        if((coinbaseUTXOs[i].prevHash == prevHash) && (coinbaseUTXOs[i].vout == vout)){
            ret = i;
        }
    }
    return ret;
}

uint32_t Verifier::returnCollectionIndex(valtype prevHash, uint32_t vout) {
    uint32_t ret = 0;
    
    for (int i = 0; i < OutCollection.size(); i++) {
        if((OutCollection[i]->prevHash == prevHash) && (OutCollection[i]->vout == vout)){
            ret = i;
        }
    }
    return ret;
}

uint32_t VerifierSync::returnCoinbaseUTXOIndex(valtype prevHash) {
    uint32_t ret = 0;
    
    for (int i = 0; i < coinbaseUTXOs.size(); i++) {
        if(coinbaseUTXOs[i].prevHash == prevHash){
            ret = i;
        }
    }
    return ret;
}
