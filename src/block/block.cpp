//
//  block.cpp
//  Bits
//
//  Created by Burak on 8.12.2022.
//

#include "block.h"



Block::Block(valtype vRawBlock, uint32_t height) {
    valtype *pVRawBlock = &vRawBlock;
    
    bool returnVal = true;
    //valtype vRawBlock = WizData::bufferAnySizeToValtype(rawBlock, size);
    std::cout << "begin." << std::endl;
    
    std::vector<Transaction> transactions;
    
    valtype vTransactionsLenFirstByte;
    valtype vTransactionsLen; int transactionsLen;
    
    int elapsedBytes = 80;
    
    if (HeaderSync::syncHeight < height){
        returnVal = false;
            std::cout << "stopke." << std::endl;
    }
    else{
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
        
        
        std::cout << "naber " << transactions[0].inputs.size() << std::endl;
        
        
        
        
        
    }
}
