//
//  transaction.cpp
//  Bits
//
//  Created by Burak on 6.12.2022.
//

#include "transaction.h"

valtype splitValtypeSet(valtype *in, int startIndex, int size){
    valtype returnValType;
    if(size > 0)
        returnValType.insert(returnValType.begin(), in->begin() + startIndex, in->begin() + startIndex + size);
    return returnValType;
}

Transaction::Transaction(valtype rawTx){
    int elapsedBytes = 0;
    bool decodeSuccess = true;
    
    valtype versionBytes = splitValtypeSet(&rawTx, elapsedBytes, 4); elapsedBytes += 4;
    this->version = *WizData::LEtoUint32(versionBytes);
    
    valtype marker = splitValtypeSet(&rawTx, elapsedBytes, 1);
    
    if(marker[0] == 0x00){
        //Witness serialization
        valtype flag = splitValtypeSet(&rawTx, elapsedBytes + 1, 1);
        if(flag[0] != 0x01)
            decodeSuccess = false;

        elapsedBytes += 2;
    }
    else {
        //nonwit
    }
    
    valtype vNumInputsFirstByte = splitValtypeSet(&rawTx, elapsedBytes, 1); elapsedBytes++;
    valtype vNumInputsLen;
    uint32_t numInputs;
    
    if(vNumInputsFirstByte[0] == 0xFD){
        vNumInputsLen = splitValtypeSet(&rawTx, elapsedBytes, 2); elapsedBytes += 2;
        numInputs = *WizData::LEtoUint32(vNumInputsLen);
    }
    else if(vNumInputsFirstByte[0] == 0xFE){
        vNumInputsLen = splitValtypeSet(&rawTx, elapsedBytes, 4); elapsedBytes += 4;
        numInputs = *WizData::LEtoUint32(vNumInputsLen);
    }
    else if(vNumInputsFirstByte[0] == 0xFF){
        vNumInputsLen = splitValtypeSet(&rawTx, elapsedBytes, 8); elapsedBytes += 8;
        numInputs = *WizData::LEtoUint32(vNumInputsLen);
    }
    else {
        numInputs = *WizData::LEtoUint32(vNumInputsFirstByte);
    }
   
    for(int i = 0; i < numInputs; i++){
        
        valtype prevOutHash = splitValtypeSet(&rawTx, elapsedBytes, 32); elapsedBytes += 32;
        valtype vVoutIndex = splitValtypeSet(&rawTx, elapsedBytes, 4); elapsedBytes += 4;
    
        uint32_t voutIndex; voutIndex = *WizData::LEtoUint32(vVoutIndex);
        
        valtype vScriptsigLenFirstByte = splitValtypeSet(&rawTx, elapsedBytes, 1); elapsedBytes++;
        valtype vScriptsigLen; uint32_t scriptsigLen;
        
        if(vScriptsigLenFirstByte[0] == 0xFD){
            vScriptsigLen = splitValtypeSet(&rawTx, elapsedBytes, 2); elapsedBytes += 2;
            scriptsigLen = *WizData::LEtoUint32(vScriptsigLen);
        }
        else if(vScriptsigLenFirstByte[0] == 0xFE){
            vScriptsigLen = splitValtypeSet(&rawTx, elapsedBytes, 4); elapsedBytes += 4;
            scriptsigLen = *WizData::LEtoUint32(vScriptsigLen);
        }
        else if(vScriptsigLenFirstByte[0] == 0xFF){
            vScriptsigLen = splitValtypeSet(&rawTx, elapsedBytes, 8); elapsedBytes += 8;
            scriptsigLen = *WizData::LEtoUint32(vScriptsigLen);
        }
        else {
            scriptsigLen = *WizData::LEtoUint32(vScriptsigLenFirstByte);
        }
        
        valtype scriptSig = splitValtypeSet(&rawTx, elapsedBytes, scriptsigLen); elapsedBytes += scriptsigLen;
        valtype vSequence = splitValtypeSet(&rawTx, elapsedBytes, 4); elapsedBytes += 4;
        uint32_t sequence = *WizData::LEtoUint32(vSequence);
    }
}

