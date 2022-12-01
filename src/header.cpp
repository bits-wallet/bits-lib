//
//  header.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#include "header.h"
#include "parser/parser.h"

unsigned int Header::syncHeight = 0;

// We can construct header from raw bytes
Header::Header(valtype rawHeader){
    HeaderParser *hp = new HeaderParser(rawHeader);
    
    uint32_t version = WizData::LEtoUint32(hp->versionParsed);
    valtype prevHash = hp->prevHashParsed;
    valtype merkeRoot = hp->merkeRootParsed;
    uint32_t timestamp = WizData::LEtoUint32(hp->timestampParsed);
    uint32_t bits = WizData::LEtoUint32(hp->bitsParsed);
    uint32_t nonce = WizData::LEtoUint32(hp->nonceParsed);
    
    delete hp;
           
    setHeader(version, prevHash, merkeRoot, timestamp, bits, nonce);
}

// We can construct header from 6 components
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
