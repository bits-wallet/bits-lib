//
//  proof.cpp
//  Bits
//
//  Created by Burak on 9.12.2022.
//

#include "proof.h"

bool Proof::importUTXOs (valtype rawImport) {
    unsigned int elapsedBytes = 0;
    valtype *pRawImport = &rawImport;
    valtype vTotalLenFirstByte = WizData::splitValtypeSet(pRawImport, elapsedBytes, 1); elapsedBytes ++;
    valtype vTotalLen; uint32_t totalLen;
    
    if(vTotalLenFirstByte[0] == 0xFD){
        vTotalLen = WizData::splitValtypeSet(pRawImport, elapsedBytes, 2); elapsedBytes += 2;
        totalLen = *WizData::LEtoUint32(vTotalLen);
    }
    else {
        totalLen = *WizData::LEtoUint32(vTotalLenFirstByte);
    }

    for (int i = 0; i < totalLen; i++) {
        uint32_t height = *WizData::LEtoUint32(WizData::splitValtypeSet(pRawImport, elapsedBytes, 4)); elapsedBytes += 4;
        
        valtype prevHash = WizData::splitValtypeSet(pRawImport, elapsedBytes, 32); elapsedBytes += 32;
        uint32_t vout = *WizData::LEtoUint32(WizData::splitValtypeSet(pRawImport, elapsedBytes, 4)); elapsedBytes += 4;
        uint64_t value = *WizData::LEtoUint64(WizData::splitValtypeSet(pRawImport, elapsedBytes, 8)); elapsedBytes += 8;
        
        valtype vScriptPubkeyLen; uint32_t scriptPubkeyLen;
        valtype vScriptPubkeyLenFirstByte = WizData::splitValtypeSet(pRawImport, elapsedBytes, 1); elapsedBytes ++;
        
        if(vScriptPubkeyLenFirstByte[0] == 0xFD){
            vScriptPubkeyLen = WizData::splitValtypeSet(pRawImport, elapsedBytes, 2); elapsedBytes += 2;
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
        }
        else if(vScriptPubkeyLenFirstByte[0] == 0xFE){
            vScriptPubkeyLen = WizData::splitValtypeSet(pRawImport, elapsedBytes, 4); elapsedBytes += 4;
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
        }
        else if(vScriptPubkeyLenFirstByte[0] == 0xFF){
            vScriptPubkeyLen = WizData::splitValtypeSet(pRawImport, elapsedBytes, 8); elapsedBytes += 8;
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
        }
        else {
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLenFirstByte);
        }
        
        valtype scriptPubkey = WizData::splitValtypeSet(pRawImport, elapsedBytes, scriptPubkeyLen); elapsedBytes += scriptPubkeyLen;
        this->utxos.push_back(UTXO(height, prevHash, vout, value, scriptPubkey));
    }
    return (elapsedBytes == rawImport.size());
}

valtype Proof::exportUTXOsRaw (std::vector<UTXO> utxos) {
    valtype returnValtype;
    valtype numUTXOs = WizData::prefixCompactSizeCast((uint32_t)utxos.size());
    returnValtype.insert(returnValtype.begin(), numUTXOs.begin(), numUTXOs.end());
    
    for(int i = 0; i < utxos.size(); i++) {
        valtype UTXOfield;
        valtype UTXOScriptPubkey = utxos[i].scriptPubkey;
        valtype scriptPubkeyLen = WizData::prefixCompactSizeCast((uint32_t)(UTXOScriptPubkey.size()));
        
        valtype UTXOHeight = *WizData::Uint32ToLE(utxos[i].height);
        UTXOfield.insert(UTXOfield.end(), UTXOHeight.begin(), UTXOHeight.end());
        
        valtype UTXOPrevHash = utxos[i].prevHash;
        UTXOfield.insert(UTXOfield.end(), UTXOPrevHash.begin(), UTXOPrevHash.end());
        
        valtype UTXOVout = *WizData::Uint32ToLE(utxos[i].vout);
        UTXOfield.insert(UTXOfield.end(), UTXOVout.begin(), UTXOVout.end());
        
        valtype UTXOValue = *WizData::Uint64ToLE(utxos[i].value);
        UTXOfield.insert(UTXOfield.end(), UTXOValue.begin(), UTXOValue.end());
        
        UTXOfield.insert(UTXOfield.end(), scriptPubkeyLen.begin(), scriptPubkeyLen.end());
        UTXOfield.insert(UTXOfield.end(), UTXOScriptPubkey.begin(), UTXOScriptPubkey.end());
        
        returnValtype.insert(returnValtype.end(), UTXOfield.begin(), UTXOfield.end());
    }
    
    return returnValtype;
}
