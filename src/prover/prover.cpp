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
    
    //1. Collect spending utxos
    for (int i = 0; i < transactions.size(); i++) {
        if(i > 0){
        // Remove tx_i inputs from the utxo set
        for (uint32_t k = 0; k < transactions[i].inputs.size(); k++) {
            std::pair<uint32_t, UTXO*> spendingUTXO;
            spendingUTXO = ProverSync::returnUTXOFromOutpoint(transactions[i].inputs[k].prevOutHash, transactions[i].inputs[k].voutIndex);
            spendings.push_back(*(spendingUTXO.second));
        }
        }
    }
    
    //2. setSpendingsRaw
    setSpendingsRaw();
    
    //3. Craft hash array of spendings
    for(int i = 0; i < this->spendings.size(); i++) {
        this->spendingsHashes.push_back(this->spendings[i].returnLeafHash());
    }

    std::cout << "happy" << std::endl;
    //4. UPDATE UTXO SET
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
            UTXO newUtxo(ProverSync::proverHeight + 1, transactions[i].txid, k, (transactions[i].outputs[k].amount), transactions[i].outputs[k].scriptPubkey);
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

void Prover::setSpendingsRaw() {
    if(this->spendings.size() > 0) {
    valtype returnValtype;
    valtype numUTXOs = WizData::prefixCompactSizeCast((uint32_t)this->spendings.size());
    returnValtype.insert(returnValtype.begin(), numUTXOs.begin(), numUTXOs.end());
    
    for(int i = 0; i < spendings.size(); i++) {
        valtype UTXOfield;
        valtype UTXOScriptPubkey = spendings[i].scriptPubkey;
        valtype scriptPubkeyLen = WizData::prefixCompactSizeCast((uint32_t)(UTXOScriptPubkey.size()));
        
        valtype UTXOHeight = *WizData::Uint32ToLE(spendings[i].height);
        UTXOfield.insert(UTXOfield.end(), UTXOHeight.begin(), UTXOHeight.end());
        
        valtype UTXOPrevHash = spendings[i].prevHash;
        UTXOfield.insert(UTXOfield.end(), UTXOPrevHash.begin(), UTXOPrevHash.end());
        
        valtype UTXOVout = *WizData::Uint32ToLE(spendings[i].vout);
        UTXOfield.insert(UTXOfield.end(), UTXOVout.begin(), UTXOVout.end());
        
        valtype UTXOValue = *WizData::Uint64ToLE(spendings[i].value);
        UTXOfield.insert(UTXOfield.end(), UTXOValue.begin(), UTXOValue.end());
        
        UTXOfield.insert(UTXOfield.end(), scriptPubkeyLen.begin(), scriptPubkeyLen.end());
        UTXOfield.insert(UTXOfield.end(), UTXOScriptPubkey.begin(), UTXOScriptPubkey.end());
        
        returnValtype.insert(returnValtype.end(), UTXOfield.begin(), UTXOfield.end());
    }
    this->spendingsRaw = returnValtype;
    }
}

valtype Prover::readSpendingsRaw() {
    return spendingsRaw;
}
