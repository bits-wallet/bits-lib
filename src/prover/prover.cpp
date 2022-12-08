//
//  prover.cpp
//  Bits
//
//  Created by Burak on 8.12.2022.
//

#include "prover.h"

uint32_t ProverSync::proverHeight = 0;
std::vector<UTXO> ProverSync::utxoSet;

Prover::Prover(valtype vRawBlock, uint32_t height) {
    valtype *pVRawBlock = &vRawBlock;
    
    //valtype vRawBlock = WizData::bufferAnySizeToValtype(rawBlock, size);
    std::cout << "begin." << std::endl;
    
    std::vector<Transaction> transactions;
    
    valtype vTransactionsLenFirstByte;
    valtype vTransactionsLen; int transactionsLen;
    
    int elapsedBytes = 80;
    
        std::cout << "devamke." << std::endl;
        vTransactionsLenFirstByte = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 1); elapsedBytes ++;
        
        if(vTransactionsLenFirstByte[0] == 0xFD){
            vTransactionsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 2); elapsedBytes += 2;
            transactionsLen = *WizData::LEtoUint32(vTransactionsLen);
        }
        else if(vTransactionsLenFirstByte[0] == 0xFE){
            vTransactionsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
            transactionsLen = *WizData::LEtoUint32(vTransactionsLen);
        }
        else if(vTransactionsLenFirstByte[0] == 0xFF){
            vTransactionsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 8); elapsedBytes += 8;
            transactionsLen = *WizData::LEtoUint32(vTransactionsLen);
        }
        else {
            transactionsLen = *WizData::LEtoUint32(vTransactionsLenFirstByte);
        }
        
        for (int i = 0; i < transactionsLen; i++) {
            std::cout << "checkinggg." << std::endl;
            valtype vThisTransactionsLenFirstByte;
            valtype vThisTransactionsLen; int thisTransactionsLen;
            
            vThisTransactionsLenFirstByte = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 1); elapsedBytes ++;
            
            if(vTransactionsLenFirstByte[0] == 0xFD){
                vThisTransactionsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 2); elapsedBytes += 2;
                thisTransactionsLen = *WizData::LEtoUint32(vThisTransactionsLen);
            }
            else if(vTransactionsLenFirstByte[0] == 0xFE){
                vThisTransactionsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
                thisTransactionsLen = *WizData::LEtoUint32(vThisTransactionsLen);
            }
            else if(vTransactionsLenFirstByte[0] == 0xFF){
                vThisTransactionsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 8); elapsedBytes += 8;
                thisTransactionsLen = *WizData::LEtoUint32(vThisTransactionsLen);
            }
            else {
                thisTransactionsLen = *WizData::LEtoUint32(vThisTransactionsLenFirstByte);
            }
            
            valtype thisTransaction = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, thisTransactionsLen); elapsedBytes += thisTransactionsLen;
            transactions.push_back(Transaction(thisTransaction));
        }
    
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
            UTXO newUtxo(transactions[i].txid, *WizData::Uint32ToLE(k), *WizData::Uint32ToLE(height), *WizData::Uint64ToLE(transactions[i].outputs[k].amount), transactions[i].outputs[k].scriptPubkey);
            ProverSync::utxoSet.push_back(newUtxo);
        }
    }
    
        ProverSync::proverHeight++; std::cout << "final proverHeight: " << ProverSync::proverHeight << std::endl;
    
    std::cout << "utxoset size: "<< ProverSync::utxoSet.size()<< std::endl;
    
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
