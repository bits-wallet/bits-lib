//
//  prover.cpp
//  Bits
//
//  Created by Burak on 8.12.2022.
//

#include "prover.h"
#include "../block/block.h""

uint32_t ProverSync::proverHeight = 0;
std::vector<UTXO> ProverSync::utxoSet;

Prover::Prover(valtype vRawBlock) {
    
    Block nb = Block::submitNewBlock(vRawBlock);
    std::vector<Transaction> transactions = nb.transactions;
    
    for (int i = 0; i < transactions.size(); i++) {
 
        if(i > 0){
        // Remove tx_i inputs from the utxo set
        for (uint32_t k = 0; k < transactions[i].inputs.size(); k++) {
            std::pair<uint64_t, UTXO*> removeUTXO;
            removeUTXO = ProverSync::returnUTXOFromOutpoint(transactions[i].inputs[k].prevOutHash, transactions[i].inputs[k].voutIndex);
            ProverSync::utxoSet.erase(ProverSync::utxoSet.begin() + removeUTXO.first);
            delete removeUTXO.second;
        }
        }
    
        // Add tx_i outputs to the utxo set
        for (uint32_t k = 0; k < transactions[i].outputs.size(); k++) {
            UTXO newUtxo(transactions[i].txid, *WizData::Uint32ToLE(k), *WizData::Uint32ToLE(ProverSync::proverHeight + 1), *WizData::Uint64ToLE(transactions[i].outputs[k].amount), transactions[i].outputs[k].scriptPubkey);
            ProverSync::utxoSet.push_back(newUtxo);
        }
    }
    
        ProverSync::proverHeight++;
}


std::pair<uint64_t, UTXO*> ProverSync::returnUTXOFromOutpoint(valtype prevHash, uint32_t vout) {
    std::pair<uint64_t, UTXO*> returnPair;
    for(uint64_t i = 0; i < ProverSync::utxoSet.size(); i++) {
        if((ProverSync::utxoSet[i].scriptPubkey == prevHash) && (ProverSync::utxoSet[i].vout == *WizData::Uint32ToLE(vout))) {
            returnPair.first = i;
            returnPair.second = &ProverSync::utxoSet[i];
        }
    }
    return returnPair;
}
