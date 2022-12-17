//
//  verifier.cpp
//  Bits
//
//  Created by Burak on 17.12.2022.
//

#include "verifier.h"

uint32_t VerifierSync::syncHeight;
utreexo::Pollard VerifierSync::forestState(0);

bool Verifier::verify(valtype rawBlock, valtype rawSpendings, valtype rawProof) {
    
    bool ret = true;
    
    //reorg?
    if(VerifierSync::syncHeight >= HeaderSync::syncHeight)
        ret = false;
    
    //0. Craft block template
    Block vb = Block::submitNewBlock(rawBlock);
    std::vector<Transaction> txns = vb.transactions;
    
    //1. Import UTXOs
    Proof prevouts;
    if(!prevouts.importUTXOs(rawSpendings))
        ret = false;
    
    //2. Cast proof bytes
    std::vector<uint8_t> proofBytes;
    for(int a = 0; a < rawProof.size(); a++) {
        proofBytes.push_back((uint8_t)rawProof[a]);
    }

    //3. Craft batchproof
    utreexo::BatchProof batchProof;
    batchProof.Serialize(proofBytes);
    
    //4. Verify spendings against forest state
    if(!VerifierSync::forestState.Verify(batchProof, prevouts.returnUTXOHashes()))
        ret= false;
    
    
    int elapsedPrevouts = 0;
    std::vector<Leaf> newLeaves;
    uint64_t inputSats = 0;
    uint64_t outputSats = 0;
    
    for (int i = 0; i < vb.transactions.size(); i++) {
        
        //5. Input validations
        for(int l = 0; l < vb.transactions[i].inputs.size(); l++) {
            
            if(l > 0){
               
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
                inputSats += 5000000000;
            }
            elapsedPrevouts++;
        }
        
        //6. Output validations
        for(uint32_t k = 0; k < vb.transactions[i].outputs.size(); k++) {
            UTXO newUtxo(VerifierSync::syncHeight + 1, vb.transactions[i].txid, k, (vb.transactions[i].outputs[k].amount), vb.transactions[i].outputs[k].scriptPubkey);
            newLeaves.emplace_back(newUtxo.returnLeafHash(), false);
            
            outputSats += vb.transactions[i].outputs[k].amount;
        }
        
    }
    
    //7. Update forest state
    VerifierSync::forestState.Modify(newLeaves, batchProof.GetTargets());
    
    //8. Inflation check
    if(inputSats != outputSats) {
        ret = false;
    }
    
    return ret;
}
