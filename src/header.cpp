//
//  header.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#include "header.h"

unsigned int Header::syncHeight = 0;

Header::Header(valtype rawHeader){
    assert(rawHeader.size() == 80);

    valtype versionParsed;
    versionParsed.insert(versionParsed.begin(), rawHeader.begin(), rawHeader.begin()+4);
    valtype prevHashParsed; prevHashParsed.insert(prevHashParsed.begin(), rawHeader.begin()+4, rawHeader.begin()+36);
    valtype merkeRootParsed; merkeRootParsed.insert(merkeRootParsed.begin(), rawHeader.begin()+36, rawHeader.begin()+68);
    valtype timestampParsed; timestampParsed.insert(timestampParsed.begin(), rawHeader.begin()+68, rawHeader.begin()+72);
    valtype bitsParsed; bitsParsed.insert(bitsParsed.begin(), rawHeader.begin()+72, rawHeader.begin()+76);
    valtype nonceParsed; nonceParsed.insert(nonceParsed.begin(), rawHeader.begin()+76, rawHeader.begin()+80);
    
    uint32_t version = WizData::LEtoUint32(versionParsed);
    valtype prevHash = prevHashParsed;
    valtype merkeRoot = merkeRootParsed;
    uint32_t timestamp = WizData::LEtoUint32(timestampParsed);
    uint32_t bits = WizData::LEtoUint32(bitsParsed);
    uint32_t nonce = WizData::LEtoUint32(nonceParsed);
           
    setHeader(version, prevHash, merkeRoot, timestamp, bits, nonce);
}

Header::Header(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce) {
    setHeader(version, prevHash, merkeRoot, timestamp, bits, nonce);
}

void Header::setHeader(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce){
    assert(prevHash.size() == 32 && merkeRoot.size() == 32 && version >= 1);
    
    this->height = ++syncHeight;
    this->version = version;
    this->prevHash = prevHash;
    this->merkeRoot = merkeRoot;
    this-> timestamp = timestamp;
    this->bits = bits;
    this-> nonce = nonce;
}

void Header::getInfo(){
    std::cout << "height: " <<  this->height << std::endl;
    std::cout << "version: " <<  this->version << std::endl;
    std::cout << "prevHash: " <<  (int)(this->prevHash)[2] << std::endl;
    std::cout << "merkeRoot: " <<  (int)(this->merkeRoot)[30] << std::endl;
    std::cout << "timestamp: " <<  this->timestamp << std::endl;
    std::cout << "bits: " <<  this->bits << std::endl;
    std::cout << "nonce: " <<  this->nonce << std::endl;
}
