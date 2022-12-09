//
//  prover.cpp
//  Bits
//
//  Created by Burak on 8.12.2022.
//

#include "prover.h"
#include "../block/block.h"

uint32_t ProverSync::proverHeight = 0;
std::vector<UTXO> ProverSync::utxoSet;
std::vector<Leaf> ProverSync::utxoLeafSet;

Prover::Prover(valtype vRawBlock) {
    
    Block nb = Block::submitNewBlock(vRawBlock);
    std::vector<Transaction> transactions = nb.transactions;
    
    for (int i = 0; i < transactions.size(); i++) {
 
        if(i > 0){
        // Remove tx_i inputs from the utxo set
        for (uint32_t k = 0; k < transactions[i].inputs.size(); k++) {
            std::pair<uint32_t, UTXO*> removeUTXO;
            removeUTXO = ProverSync::returnUTXOFromOutpoint(transactions[i].inputs[k].prevOutHash, transactions[i].inputs[k].voutIndex);
            ProverSync::utxoSet.erase(ProverSync::utxoSet.begin() + removeUTXO.first);
            //
            ProverSync::utxoLeafSet.erase(ProverSync::utxoLeafSet.begin() + removeUTXO.first);
            delete removeUTXO.second;
        }
        }
    
        // Add tx_i outputs to the utxo set
        for (uint32_t k = 0; k < transactions[i].outputs.size(); k++) {
            UTXO newUtxo(transactions[i].txid, k, (ProverSync::proverHeight + 1), (transactions[i].outputs[k].amount), transactions[i].outputs[k].scriptPubkey);
            ProverSync::utxoSet.push_back(newUtxo);
            ProverSync::utxoLeafSet.emplace_back(newUtxo.returnLeafHash(), false);

        }
    }
    
        ProverSync::proverHeight++;
}


std::pair<uint32_t, UTXO*> ProverSync::returnUTXOFromOutpoint(valtype prevHash, uint32_t vout) {
    std::pair<uint32_t, UTXO*> returnPair;
    for(uint32_t i = 0; i < ProverSync::utxoSet.size(); i++) {
        if((ProverSync::utxoSet[i].scriptPubkey == prevHash) && (ProverSync::utxoSet[i].vout == vout)) {
            returnPair.first = i;
            returnPair.second = &ProverSync::utxoSet[i];
        }
    }
    return returnPair;
}
