//
//  proof.cpp
//  Bits
//
//  Created by Burak on 9.12.2022.
//

#include "extract.h"

bool Exract::importUTXOsPartial (valtype *rawImport) {
    unsigned int elapsedBytes = 0;
    if(rawImport->size() > 0) {

    valtype vTotalLenFirstByte = WizData::splitValtypeSet(rawImport, elapsedBytes, 1); elapsedBytes ++;
    valtype vTotalLen; uint32_t totalLen;
    
    if(vTotalLenFirstByte[0] == 0xFD){
        vTotalLen = WizData::splitValtypeSet(rawImport, elapsedBytes, 2); elapsedBytes += 2;
        totalLen = *WizData::LEtoUint32(vTotalLen);
    }
    else {
        totalLen = *WizData::LEtoUint32(vTotalLenFirstByte);
    }

    for (int i = 0; i < totalLen; i++) {
        uint32_t height = *WizData::LEtoUint32(WizData::splitValtypeSet(rawImport, elapsedBytes, 4)); elapsedBytes += 4;
                
        uint64_t value = *WizData::LEtoUint64(WizData::splitValtypeSet(rawImport, elapsedBytes, 8)); elapsedBytes += 8;
        
        valtype vScriptPubkeyLen; uint32_t scriptPubkeyLen;
        valtype vScriptPubkeyLenFirstByte = WizData::splitValtypeSet(rawImport, elapsedBytes, 1); elapsedBytes ++;
        
        if(vScriptPubkeyLenFirstByte[0] == 0xFD){
            vScriptPubkeyLen = WizData::splitValtypeSet(rawImport, elapsedBytes, 2); elapsedBytes += 2;
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
        }
        else if(vScriptPubkeyLenFirstByte[0] == 0xFE){
            vScriptPubkeyLen = WizData::splitValtypeSet(rawImport, elapsedBytes, 4); elapsedBytes += 4;
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
        }
        else if(vScriptPubkeyLenFirstByte[0] == 0xFF){
            vScriptPubkeyLen = WizData::splitValtypeSet(rawImport, elapsedBytes, 8); elapsedBytes += 8;
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLen);
        }
        else {
            scriptPubkeyLen = *WizData::LEtoUint32(vScriptPubkeyLenFirstByte);
        }
        
        valtype scriptPubkey = WizData::splitValtypeSet(rawImport, elapsedBytes, scriptPubkeyLen); elapsedBytes += scriptPubkeyLen;
        this->partialUTXOs.push_back(PartialUTXO(height, value, scriptPubkey));
    }
    }
    return (elapsedBytes == rawImport->size());
}

bool Exract::importUTXOs (valtype rawImport) {
    unsigned int elapsedBytes = 0;
    if(rawImport.size() > 0) {
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
        this->UTXOs.push_back(UTXO(height, prevHash, vout, value, scriptPubkey));
    }
    }
    return (elapsedBytes == rawImport.size());
}

valtype Exract::exportUTXOs(std::vector<UTXO> importUTXOs){
    valtype returnValtype;
    
    if(importUTXOs.size() > 0) {
        
    valtype numUTXOs = WizData::prefixCompactSizeCast((uint32_t)importUTXOs.size());
    returnValtype.insert(returnValtype.begin(), numUTXOs.begin(), numUTXOs.end());
        
    for(int i = 0; i < importUTXOs.size(); i++) {
        valtype UTXOfield;
        valtype UTXOScriptPubkey = importUTXOs[i].scriptPubkey;
        valtype scriptPubkeyLen = WizData::prefixCompactSizeCast((uint32_t)(UTXOScriptPubkey.size()));
        
        valtype UTXOHeight = *WizData::Uint32ToLE(importUTXOs[i].height);
        UTXOfield.insert(UTXOfield.end(), UTXOHeight.begin(), UTXOHeight.end());
        
        valtype UTXOPrevHash = importUTXOs[i].prevHash;
        UTXOfield.insert(UTXOfield.end(), UTXOPrevHash.begin(), UTXOPrevHash.end());
        
        valtype UTXOVout = *WizData::Uint32ToLE(importUTXOs[i].vout);
        UTXOfield.insert(UTXOfield.end(), UTXOVout.begin(), UTXOVout.end());
        
        valtype UTXOValue = *WizData::Uint64ToLE(importUTXOs[i].value);
        UTXOfield.insert(UTXOfield.end(), UTXOValue.begin(), UTXOValue.end());
        
        UTXOfield.insert(UTXOfield.end(), scriptPubkeyLen.begin(), scriptPubkeyLen.end());
        UTXOfield.insert(UTXOfield.end(), UTXOScriptPubkey.begin(), UTXOScriptPubkey.end());
        
        returnValtype.insert(returnValtype.end(), UTXOfield.begin(), UTXOfield.end());
    }
    }
    return returnValtype;
}

std::vector<Hash> Exract::returnUTXOHashes() {
    std::vector<Hash> ret;
    for(int i = 0; i < this->UTXOs.size(); i++) {
        ret.push_back(this->UTXOs[i].returnLeafHash());
    }
    return ret;
}

std::vector<Leaf> Exract::returnUTXOLeaves() {
    std::vector<Leaf> ret;
    for(int i = 0; i < this->UTXOs.size(); i++) {
        ret.emplace_back(this->UTXOs[i].returnLeafHash(), false);
    }
    return ret;
}
