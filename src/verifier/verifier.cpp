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
    
    //1. Craft block template
    Block vb = Block::submitNewBlock(rawBlock);
    
    //2. Merkle root match
    std::vector<Bytes> txIDs;
    for(int i = 0; i < vb.transactions.size(); i++){ txIDs.push_back(vb.transactions[i].txid); }
    if(returnMerkleRoot(txIDs) != Header::getHeaderMerkeRoot(VerifierSync::syncHeight + 1))
        ret = false;
    
    //3. Import UTXOs
    Proof prevouts;
    if(!prevouts.importUTXOs(rawSpendings))
        ret = false;
    
    //4. Craft batchproof
    utreexo::BatchProof batchProof;
    batchProof.Serialize(proofBytes);
    
    //5. Verify spendings against forest state
    if(!VerifierSync::forestState.Verify(batchProof, prevouts.returnUTXOHashes()))
        ret= false;
    
    int elapsedPrevouts = 0;
    std::vector<Leaf> newLeaves;
    uint64_t inputSats = 0;
    uint64_t outputSats = 0;
    
    for (int i = 0; i < vb.transactions.size(); i++) {
        
        //6. Input validations
        for(int l = 0; l < vb.transactions[i].inputs.size(); l++) {
            
            if(i > 0){
               
            //a. Outpoint matching
            if(!((vb.transactions[i].inputs[l].prevOutHash == prevouts.utxos[elapsedPrevouts].prevHash) &&
                 (vb.transactions[i].inputs[l].voutIndex == prevouts.utxos[elapsedPrevouts].vout)))
                ret= false;
            
            //b. Libbitcoin-consensus::EvalScript()...
            
            //c. Increment input sats
            inputSats += prevouts.utxos[elapsedPrevouts].value;

            //n. Increment 'elapsedPrevouts'
            }
            else {
                //Coinbase input validation
                if (vb.transactions[i].inputs.size() != 1)
                    ret= false;
                
                inputSats += GetBlockSubsidy(VerifierSync::syncHeight + 1);
            }
            elapsedPrevouts++;
        }

        //7. Output validations
        for(uint32_t k = 0; k < vb.transactions[i].outputs.size(); k++) {
            
            //Add new utxos to leaf set
            UTXO newUtxo(VerifierSync::syncHeight + 1, vb.transactions[i].txid, k, (vb.transactions[i].outputs[k].amount), vb.transactions[i].outputs[k].scriptPubkey);
            newLeaves.emplace_back(newUtxo.returnLeafHash(), false);
            
            //Increment output sats
            outputSats += vb.transactions[i].outputs[k].amount;
        }
    }
    
    //8. Inflation check
    if(inputSats != outputSats) {
        ret = false;
    }

    if(!ret)
        return ret;
    
    //AFTER VALIDATIONS
    
    //9. Free previos header from memory
    if ((((Header*)Header::headerAddresses[0])->height) < (VerifierSync::syncHeight + 1)) {
        delete (Header*)Header::headerAddresses[0];
        Header::headerAddresses.erase(Header::headerAddresses.begin());
    }

    //10. Update forest state
    VerifierSync::forestState.Modify(newLeaves, batchProof.GetTargets());
    
    //11. Increment sync height
    VerifierSync::syncHeight++;
    
    std::cout << "inputSats: " << inputSats << std::endl;
    std::cout << "outputSats: " << outputSats << std::endl;
    
    return ret;
}

VerifierSync::VerifierSync(uint32_t startHeight) {
    this->syncHeight = startHeight;
    this->forestState = utreexo::Pollard(0);
}

VerifierSync::VerifierSync(uint32_t startHeight, uint64_t numLeaves, uint64_t numRoots, std::array<unsigned char, 1024>roots) {
    std::vector<Hash> hashes;
    int elapsed = 0;
    
    for (int i = 0; i < numRoots; i++) {
        Hash hash;
        for (int k = 0; k < 32; k++) {
            hash[k] =(uint8_t)roots[elapsed];
        }
        hashes.push_back(hash);
    }
    
    this->forestState = utreexo::Pollard(hashes, numLeaves);
    this->syncHeight = startHeight;
}

std::array<unsigned char, 1024> VerifierSync::getRoots() {
    std::array<unsigned char, 1024> ret;
    
    std::vector<Hash> roots;
    forestState.Roots(roots);
    
    int elapsed = 0;
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
