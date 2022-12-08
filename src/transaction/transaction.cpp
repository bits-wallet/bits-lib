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
    std::vector<TxIn> inputs;
    std::vector<TxOut> outputs;
    
    valtype *pRawTx = &rawTx;
    
    int elapsedBytes = 0;
    int elapsedBytesUntilWitness = 0;
    bool decodeSuccess = true;
    bool witnessSer = true;
    
    valtype versionBytes = splitValtypeSet(pRawTx, elapsedBytes, 4); elapsedBytes += 4;
    valtype marker = splitValtypeSet(pRawTx, elapsedBytes, 1);
    
    if(marker[0] == 0x00){
        //Witness serialization
        valtype flag = splitValtypeSet(pRawTx, elapsedBytes + 1, 1);
        if(flag[0] != 0x01)
            decodeSuccess = false;

        elapsedBytes += 2;
    }
    else {
        //nonwit
        witnessSer = false;
    }
    
    valtype vNumInputsFirstByte = splitValtypeSet(pRawTx, elapsedBytes, 1); elapsedBytes++;
    valtype vNumInputsLen;
    uint32_t numInputs;
    
    if(vNumInputsFirstByte[0] == 0xFD){
        vNumInputsLen = splitValtypeSet(pRawTx, elapsedBytes, 2); elapsedBytes += 2;
        numInputs = *WizData::LEtoUint32(vNumInputsLen);
    }
    else if(vNumInputsFirstByte[0] == 0xFE){
        vNumInputsLen = splitValtypeSet(pRawTx, elapsedBytes, 4); elapsedBytes += 4;
        numInputs = *WizData::LEtoUint32(vNumInputsLen);
    }
    else if(vNumInputsFirstByte[0] == 0xFF){
        vNumInputsLen = splitValtypeSet(pRawTx, elapsedBytes, 8); elapsedBytes += 8;
        numInputs = *WizData::LEtoUint32(vNumInputsLen);
    }
    else {
        numInputs = *WizData::LEtoUint32(vNumInputsFirstByte);
    }
   
    for(int i = 0; i < numInputs; i++){
        valtype prevOutHash = splitValtypeSet(pRawTx, elapsedBytes, 32); elapsedBytes += 32;
        valtype vVoutIndex = splitValtypeSet(pRawTx, elapsedBytes, 4); elapsedBytes += 4;
    
        uint32_t voutIndex; voutIndex = *WizData::LEtoUint32(vVoutIndex);
        
        valtype vScriptsigLenFirstByte = splitValtypeSet(pRawTx, elapsedBytes, 1); elapsedBytes++;
        valtype vScriptsigLen; uint32_t scriptsigLen;
        
        if(vScriptsigLenFirstByte[0] == 0xFD){
            vScriptsigLen = splitValtypeSet(pRawTx, elapsedBytes, 2); elapsedBytes += 2;
            scriptsigLen = *WizData::LEtoUint32(vScriptsigLen);
        }
        else if(vScriptsigLenFirstByte[0] == 0xFE){
            vScriptsigLen = splitValtypeSet(pRawTx, elapsedBytes, 4); elapsedBytes += 4;
            scriptsigLen = *WizData::LEtoUint32(vScriptsigLen);
        }
        else if(vScriptsigLenFirstByte[0] == 0xFF){
            vScriptsigLen = splitValtypeSet(pRawTx, elapsedBytes, 8); elapsedBytes += 8;
            scriptsigLen = *WizData::LEtoUint32(vScriptsigLen);
        }
        else {
            scriptsigLen = *WizData::LEtoUint32(vScriptsigLenFirstByte);
        }
        
        valtype scriptSig = splitValtypeSet(pRawTx, elapsedBytes, scriptsigLen); elapsedBytes += scriptsigLen;
        valtype vSequence = splitValtypeSet(pRawTx, elapsedBytes, 4); elapsedBytes += 4;
        uint32_t sequence = *WizData::LEtoUint32(vSequence);
        TxIn newTxIn(prevOutHash, voutIndex, scriptSig, sequence);
        inputs.push_back(newTxIn);
    }
    
    valtype vNumOutputsFirstByte = splitValtypeSet(pRawTx, elapsedBytes, 1); elapsedBytes++;
    valtype vNumOutputsLen;
    uint32_t numOutputs;
    
    if(vNumInputsFirstByte[0] == 0xFD){
        vNumOutputsLen = splitValtypeSet(pRawTx, elapsedBytes, 2); elapsedBytes += 2;
        numOutputs = *WizData::LEtoUint32(vNumInputsLen);
    }
    else if(vNumInputsFirstByte[0] == 0xFE){
        vNumOutputsLen = splitValtypeSet(pRawTx, elapsedBytes, 4); elapsedBytes += 4;
        numOutputs = *WizData::LEtoUint32(vNumOutputsLen);
    }
    else if(vNumInputsFirstByte[0] == 0xFF){
        vNumOutputsLen = splitValtypeSet(pRawTx, elapsedBytes, 8); elapsedBytes += 8;
        numOutputs = *WizData::LEtoUint32(vNumOutputsLen);
    }
    else {
        numOutputs = *WizData::LEtoUint32(vNumOutputsFirstByte);
    }
    
    for(int i = 0; i < numOutputs; i++){
        valtype vAmount = splitValtypeSet(pRawTx, elapsedBytes, 8); elapsedBytes += 8;
        uint64_t amount = *WizData::LEtoUint64(vAmount);
        
        valtype vScriptPubkeyLenFirstByte = splitValtypeSet(pRawTx, elapsedBytes, 1); elapsedBytes++;
        valtype vScriptPubkeyLen; uint32_t scriptPubkeyLen;
        
        if(vScriptPubkeyLenFirstByte[0] == 0xFD){
            vScriptPubkeyLen = splitValtypeSet(pRawTx, elapsedBytes, 2); elapsedBytes += 2;
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
        }
        else if(vScriptPubkeyLenFirstByte[0] == 0xFE){
            vScriptPubkeyLen = splitValtypeSet(pRawTx, elapsedBytes, 4); elapsedBytes += 4;
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
        }
        else if(vScriptPubkeyLenFirstByte[0] == 0xFF){
            vScriptPubkeyLen = splitValtypeSet(pRawTx, elapsedBytes, 8); elapsedBytes += 8;
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
        }
        else {
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLenFirstByte);
        }
        
        valtype scriptPubkey = splitValtypeSet(pRawTx, elapsedBytes, scriptPubkeyLen); elapsedBytes += scriptPubkeyLen;
        
        TxOut newTxOut(amount, scriptPubkey);
        outputs.push_back(newTxOut);
    }
    
    if(witnessSer) {
        elapsedBytesUntilWitness = elapsedBytes;
        //witness ser
        for (int i = 0; i < inputs.size(); i++) {
            valtype vThisInElementCountFirstByte = splitValtypeSet(pRawTx, elapsedBytes, 1); elapsedBytes++;
            valtype vThisInElementCount; uint32_t thisInElementCount;
            if(vThisInElementCountFirstByte[0] == 0xFD){
                vThisInElementCount = splitValtypeSet(pRawTx, elapsedBytes, 2); elapsedBytes += 2;
                thisInElementCount = *WizData::LEtoUint32(vThisInElementCount);
            }
            else if(vThisInElementCountFirstByte[0] == 0xFE){
                vThisInElementCount = splitValtypeSet(pRawTx, elapsedBytes, 4); elapsedBytes += 4;
                thisInElementCount = *WizData::LEtoUint32(vThisInElementCount);
            }
            else if(vThisInElementCountFirstByte[0] == 0xFF){
                vThisInElementCount = splitValtypeSet(pRawTx, elapsedBytes, 8); elapsedBytes += 8;
                thisInElementCount = *WizData::LEtoUint32(vThisInElementCount);
            }
            else {
                thisInElementCount = *WizData::LEtoUint32(vThisInElementCountFirstByte);
            }
            std::vector<valtype> inputElements;
            
            for (int i = 0; i < thisInElementCount; i++) {
                valtype element;
                valtype vElementLenFirstByte = splitValtypeSet(pRawTx, elapsedBytes, 1); elapsedBytes++;
                valtype vElementLen; uint32_t elementLen;
                
                if(vElementLenFirstByte[0] == 0xFD){
                    vElementLen = splitValtypeSet(pRawTx, elapsedBytes, 2); elapsedBytes += 2;
                    elementLen = *WizData::LEtoUint32(vElementLen);
                }
                else if(vElementLenFirstByte[0] == 0xFE){
                    vElementLen = splitValtypeSet(pRawTx, elapsedBytes, 4); elapsedBytes += 4;
                    elementLen = *WizData::LEtoUint32(vElementLen);
                }
                else if(vElementLenFirstByte[0] == 0xFF){
                    vElementLen = splitValtypeSet(pRawTx, elapsedBytes, 8); elapsedBytes += 8;
                    elementLen = *WizData::LEtoUint32(vElementLen);
                }
                else {
                    elementLen = *WizData::LEtoUint32(vElementLenFirstByte);
                }
                if(elementLen > 0)
                    element = splitValtypeSet(pRawTx, elapsedBytes, elementLen); elapsedBytes += elementLen;
                inputElements.push_back(element);
            }
 
               inputs[i].witness = inputElements;

        } //input times
    }

    valtype vLocktime = splitValtypeSet(pRawTx, elapsedBytes, 4); elapsedBytes += 4;
    
    valtype *wtxid = new valtype(32);
    CSHA256().Write(pRawTx->data(), pRawTx->size()).Finalize(wtxid->data());
    CSHA256().Write(wtxid->data(), wtxid->size()).Finalize(wtxid->data());

    valtype *baseRawTx = new valtype;

    baseRawTx->insert(baseRawTx->begin(), versionBytes.begin(), versionBytes.end());

    int rawTxBeginOffset = 4;
    if(witnessSer)
        rawTxBeginOffset = 6;
    baseRawTx->insert(baseRawTx->end(), pRawTx->begin() + rawTxBeginOffset, pRawTx->begin() + elapsedBytesUntilWitness);
    baseRawTx->insert(baseRawTx->end(), vLocktime.begin(), vLocktime.end());

    valtype *txid = new valtype(32);
    CSHA256().Write(baseRawTx->data(), baseRawTx->size()).Finalize((*txid).data());
    CSHA256().Write((*txid).data(), (*txid).size()).Finalize((*txid).data());
    delete baseRawTx;
    
    if(elapsedBytes != rawTx.size())
        decodeSuccess = false;
    
    if(decodeSuccess){
        this->version = *WizData::LEtoUint32(versionBytes);
        this->inputs = inputs;
        this->outputs = outputs;
        this->locktime = *WizData::LEtoUint32(vLocktime);
        this->txid = *txid;
        this->wtxid = *wtxid;
    }
}

