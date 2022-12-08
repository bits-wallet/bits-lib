//
//  block.cpp
//  Bits
//
//  Created by Burak on 8.12.2022.
//

#include "block.h"



Block Block::submitNewBlock(valtype vRawBlock, uint32_t height) {
    valtype *pVRawBlock = &vRawBlock;
    
    bool returnVal = true;
    //valtype vRawBlock = WizData::bufferAnySizeToValtype(rawBlock, size);
    std::cout << "begin." << std::endl;
    
    bool decodeSuccess = true;
    std::vector<Transaction> transactions;
    
    valtype vTransactionsLenFirstByte;
    valtype vTransactionsLen; int transactionsLen;
    
    int elapsedBytes = 80;
    
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
            ///
            std::vector<TxIn> inputs;
            std::vector<TxOut> outputs;

            //int elapsedBytes = 0;
            int elapsedBytesUntilWitness = 0;
            int elapsedBytesUntilBeginning = 0;
            
            bool witnessSer = true;
            
            elapsedBytesUntilBeginning = elapsedBytes;
            valtype versionBytes = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
            valtype marker = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 1);
            
            if(marker[0] == 0x00){
                //Witness serialization
                valtype flag = WizData::splitValtypeSet(pVRawBlock, elapsedBytes + 1, 1);
                if(flag[0] != 0x01)
                    decodeSuccess = false;

                elapsedBytes += 2;
            }
            else {
                //nonwit
                witnessSer = false;
            }
            
            valtype vNumInputsFirstByte = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 1); elapsedBytes++;
            valtype vNumInputsLen;
            uint32_t numInputs;
            
            if(vNumInputsFirstByte[0] == 0xFD){
                vNumInputsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 2); elapsedBytes += 2;
                numInputs = *WizData::LEtoUint32(vNumInputsLen);
            }
            else if(vNumInputsFirstByte[0] == 0xFE){
                vNumInputsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
                numInputs = *WizData::LEtoUint32(vNumInputsLen);
            }
            else if(vNumInputsFirstByte[0] == 0xFF){
                vNumInputsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 8); elapsedBytes += 8;
                numInputs = *WizData::LEtoUint32(vNumInputsLen);
            }
            else {
                numInputs = *WizData::LEtoUint32(vNumInputsFirstByte);
            }
           
            for(int t = 0; t < numInputs; t++){
                valtype prevOutHash = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 32); elapsedBytes += 32;
                valtype vVoutIndex = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
            
                uint32_t voutIndex; voutIndex = *WizData::LEtoUint32(vVoutIndex);
                
                valtype vScriptsigLenFirstByte = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 1); elapsedBytes++;
                valtype vScriptsigLen; uint32_t scriptsigLen;
                
                if(vScriptsigLenFirstByte[0] == 0xFD){
                    vScriptsigLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 2); elapsedBytes += 2;
                    scriptsigLen = *WizData::LEtoUint32(vScriptsigLen);
                }
                else if(vScriptsigLenFirstByte[0] == 0xFE){
                    vScriptsigLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
                    scriptsigLen = *WizData::LEtoUint32(vScriptsigLen);
                }
                else if(vScriptsigLenFirstByte[0] == 0xFF){
                    vScriptsigLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 8); elapsedBytes += 8;
                    scriptsigLen = *WizData::LEtoUint32(vScriptsigLen);
                }
                else {
                    scriptsigLen = *WizData::LEtoUint32(vScriptsigLenFirstByte);
                }
                
                valtype scriptSig = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, scriptsigLen); elapsedBytes += scriptsigLen;
                valtype vSequence = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
                uint32_t sequence = *WizData::LEtoUint32(vSequence);
                
                TxIn newTxIn(prevOutHash, voutIndex, scriptSig, sequence);
                inputs.push_back(newTxIn);
            }
            
            valtype vNumOutputsFirstByte = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 1); elapsedBytes++;
            valtype vNumOutputsLen;
            uint32_t numOutputs;
            
            if(vNumInputsFirstByte[0] == 0xFD){
                vNumOutputsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 2); elapsedBytes += 2;
                numOutputs = *WizData::LEtoUint32(vNumInputsLen);
            }
            else if(vNumInputsFirstByte[0] == 0xFE){
                vNumOutputsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
                numOutputs = *WizData::LEtoUint32(vNumOutputsLen);
            }
            else if(vNumInputsFirstByte[0] == 0xFF){
                vNumOutputsLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 8); elapsedBytes += 8;
                numOutputs = *WizData::LEtoUint32(vNumOutputsLen);
            }
            else {
                numOutputs = *WizData::LEtoUint32(vNumOutputsFirstByte);
            }
            
            for(int i = 0; i < numOutputs; i++){
                valtype vAmount = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 8); elapsedBytes += 8;
                uint64_t amount = *WizData::LEtoUint64(vAmount);
                
                valtype vScriptPubkeyLenFirstByte = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 1); elapsedBytes++;
                valtype vScriptPubkeyLen; uint32_t scriptPubkeyLen;
                
                if(vScriptPubkeyLenFirstByte[0] == 0xFD){
                    vScriptPubkeyLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 2); elapsedBytes += 2;
                    scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
                }
                else if(vScriptPubkeyLenFirstByte[0] == 0xFE){
                    vScriptPubkeyLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
                    scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
                }
                else if(vScriptPubkeyLenFirstByte[0] == 0xFF){
                    vScriptPubkeyLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 8); elapsedBytes += 8;
                    scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
                }
                else {
                    scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLenFirstByte);
                }
                
                valtype scriptPubkey = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, scriptPubkeyLen); elapsedBytes += scriptPubkeyLen;
                
                TxOut newTxOut(amount, scriptPubkey);
                outputs.push_back(newTxOut);
            }
            
            elapsedBytesUntilWitness = elapsedBytes;
            
            if(witnessSer) {
                //witness ser
                for (int i = 0; i < inputs.size(); i++) {
                    valtype vThisInElementCountFirstByte = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 1); elapsedBytes++;
                    valtype vThisInElementCount; uint32_t thisInElementCount;
                    if(vThisInElementCountFirstByte[0] == 0xFD){
                        vThisInElementCount = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 2); elapsedBytes += 2;
                        thisInElementCount = *WizData::LEtoUint32(vThisInElementCount);
                    }
                    else if(vThisInElementCountFirstByte[0] == 0xFE){
                        vThisInElementCount = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
                        thisInElementCount = *WizData::LEtoUint32(vThisInElementCount);
                    }
                    else if(vThisInElementCountFirstByte[0] == 0xFF){
                        vThisInElementCount = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 8); elapsedBytes += 8;
                        thisInElementCount = *WizData::LEtoUint32(vThisInElementCount);
                    }
                    else {
                        thisInElementCount = *WizData::LEtoUint32(vThisInElementCountFirstByte);
                    }
                    std::vector<valtype> inputElements;
                    
                    for (int i = 0; i < thisInElementCount; i++) {
                        valtype element;
                        valtype vElementLenFirstByte = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 1); elapsedBytes++;
                        valtype vElementLen; uint32_t elementLen;
                        
                        if(vElementLenFirstByte[0] == 0xFD){
                            vElementLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 2); elapsedBytes += 2;
                            elementLen = *WizData::LEtoUint32(vElementLen);
                        }
                        else if(vElementLenFirstByte[0] == 0xFE){
                            vElementLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
                            elementLen = *WizData::LEtoUint32(vElementLen);
                        }
                        else if(vElementLenFirstByte[0] == 0xFF){
                            vElementLen = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 8); elapsedBytes += 8;
                            elementLen = *WizData::LEtoUint32(vElementLen);
                        }
                        else {
                            elementLen = *WizData::LEtoUint32(vElementLenFirstByte);
                        }
                        if(elementLen > 0)
                            element = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, elementLen); elapsedBytes += elementLen;
                        inputElements.push_back(element);
                    }
         
                       inputs[i].witness = inputElements;

                } //input times
            }

            valtype vLocktime = WizData::splitValtypeSet(pVRawBlock, elapsedBytes, 4); elapsedBytes += 4;
            
            valtype fulltx; fulltx.insert(fulltx.begin(), pVRawBlock->begin() + elapsedBytesUntilBeginning, pVRawBlock->begin() + elapsedBytes);
            
            valtype *wtxid = new valtype(32);
            CSHA256().Write(fulltx.data(), fulltx.size()).Finalize(wtxid->data());
            CSHA256().Write(wtxid->data(), wtxid->size()).Finalize(wtxid->data());

            valtype *baseRawTx = new valtype;
            valtype *txid = new valtype(32);

            if(witnessSer) {
            baseRawTx->insert(baseRawTx->begin(), versionBytes.begin(), versionBytes.end());
            int rawTxBeginOffset = 4;
            if(witnessSer)
                rawTxBeginOffset = 6;
            baseRawTx->insert(baseRawTx->end(), pVRawBlock->begin() + elapsedBytesUntilBeginning + rawTxBeginOffset, pVRawBlock->begin() + elapsedBytesUntilWitness);
            baseRawTx->insert(baseRawTx->end(), vLocktime.begin(), vLocktime.end());
            
            CSHA256().Write(baseRawTx->data(), baseRawTx->size()).Finalize((*txid).data());
            CSHA256().Write((*txid).data(), (*txid).size()).Finalize((*txid).data());
            }
            else {
                txid = wtxid;
            }
            
            if(decodeSuccess){
                Transaction newtx(*txid, *wtxid, *WizData::LEtoUint32(versionBytes), inputs, outputs, *WizData::LEtoUint32(vLocktime));
                transactions.push_back(newtx);
            }

            delete baseRawTx; delete wtxid;
            
            if(witnessSer)
                delete txid;
            
           
            ///
        }
    
    if(elapsedBytes != pVRawBlock->size())
        decodeSuccess = false;
    
    Block newBlock;
    
    if(decodeSuccess)
        newBlock.setBlock(transactions);
    
    std::cout << "elapsedBytes " << elapsedBytes << std::endl;
    
   
    std::cout << "block decodeSuccess " << decodeSuccess << std::endl;
        
    std::cout << "number of total txns " << transactions.size() << std::endl;
    
    return newBlock;
}
