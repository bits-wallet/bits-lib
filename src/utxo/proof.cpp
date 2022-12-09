//
//  proof.cpp
//  Bits
//
//  Created by Burak on 9.12.2022.
//

#include "proof.h"

void Proof::importUtxos (valtype rawImport) {
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
        
    }
    
    
}



