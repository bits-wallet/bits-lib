//
//  headerParser.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#include "parser.h"

HeaderParser::HeaderParser(valtype rawHeader){
    assert(rawHeader.size() == 80);
    this->versionParsed.insert(versionParsed.begin(), rawHeader.begin(), rawHeader.begin()+4);
    this->prevHashParsed.insert(prevHashParsed.begin(), rawHeader.begin()+4, rawHeader.begin()+36);
    this->merkeRootParsed.insert(merkeRootParsed.begin(), rawHeader.begin()+36, rawHeader.begin()+68);
    this->timestampParsed.insert(timestampParsed.begin(), rawHeader.begin()+68, rawHeader.begin()+72);
    this->bitsParsed.insert(bitsParsed.begin(), rawHeader.begin()+72, rawHeader.begin()+76);
    this->nonceParsed.insert(nonceParsed.begin(), rawHeader.begin()+76, rawHeader.begin()+80);
}

HeaderConstructor::HeaderConstructor(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce){
    assert(prevHash.size() == 32 && merkeRoot.size() == 32 && version >= 1);
    
    valtype *pVersion = WizData::Uint32ToLE(version);
    valtype *pTimestamp = WizData::Uint32ToLE(timestamp);
    valtype *pBits = WizData::Uint32ToLE(bits);
    valtype *pNonce = WizData::Uint32ToLE(nonce);
    valtype rawHeader;
    
    rawHeader.insert(rawHeader.begin(), pVersion->begin(), pVersion->end());
    rawHeader.insert(rawHeader.begin()+4, prevHash.begin(), prevHash.end());
    rawHeader.insert(rawHeader.begin()+36, merkeRoot.begin(), merkeRoot.end());
    rawHeader.insert(rawHeader.begin()+68, pTimestamp->begin(), pTimestamp->end());
    rawHeader.insert(rawHeader.begin()+72, pBits->begin(), pBits->end());
    rawHeader.insert(rawHeader.begin()+76, pNonce->begin(), pNonce->end());
    
    this->rawHeader.insert(this->rawHeader.begin(), rawHeader.begin(), rawHeader.end());
    delete pVersion; delete pTimestamp; delete pBits; delete pNonce;
}
