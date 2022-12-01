//
//  header.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#include "header.h"
#include "parser/parser.h"

unsigned int Header::syncHeight = 0;

// We can construct header from raw bytes 80
Header::Header(valtype rawHeader){
    
    valtype blockHash(32);
    CSHA256().Write(rawHeader.data(), rawHeader.size()).Finalize(blockHash.data());
    CSHA256().Write(blockHash.data(), blockHash.size()).Finalize(blockHash.data());
    
    HeaderParser *hp = new HeaderParser(rawHeader);
    setHeader(
              WizData::LEtoUint32(hp->versionParsed), //Parse header version 4-byte LE
              hp->prevHashParsed, //Parse previos block hash 32-byte LE
              hp->merkeRootParsed, //Parse merkle root 32-byte LE
              WizData::LEtoUint32(hp->timestampParsed), //Parse timestamp 4-byte LE
              WizData::LEtoUint32(hp->bitsParsed), //Parse bits 4-byte LE
              WizData::LEtoUint32(hp->nonceParsed), //Parse nonce 4-byte LE
              blockHash
              );

    delete hp;
}

// We can construct header from 6 components
Header::Header(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce) {
    setHeader(version, prevHash, merkeRoot, timestamp, bits, nonce, merkeRoot);
}

void Header::setHeader(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce, valtype blockHash){
    assert(prevHash.size() == 32 && merkeRoot.size() == 32 && version >= 1);

    this->hash = blockHash;
    this->height = ++syncHeight;
    this->version = version;
    this->prevHash = prevHash;
    this->merkeRoot = merkeRoot;
    this-> timestamp = timestamp;
    this->bits = bits;
    this-> nonce = nonce;
}

void Header::getInfo(){
    std::cout << "hash: " <<  (int)(this->hash)[24] << std::endl;
    
    std::cout << "height: " <<  this->height << std::endl;
    std::cout << "version: " <<  this->version << std::endl;
    std::cout << "prevHash: " <<  (int)(this->prevHash)[2] << std::endl;
    std::cout << "merkeRoot: " <<  (int)(this->merkeRoot)[30] << std::endl;
    std::cout << "timestamp: " <<  this->timestamp << std::endl;
    std::cout << "bits: " <<  this->bits << std::endl;
    std::cout << "nonce: " <<  this->nonce << std::endl;
}
