//
//  header.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#include "header.h"
#include "parser/parser.h"

uint32_t Header::syncHeight = -1;
std::vector<uint64_t> Header::headerAddresses;

// We can construct header from raw bytes 80
Header::Header(valtype rawHeader){
    valtype *blockHash = new valtype(32);
    CSHA256().Write(rawHeader.data(), rawHeader.size()).Finalize((*blockHash).data());
    CSHA256().Write((*blockHash).data(), (*blockHash).size()).Finalize((*blockHash).data());
    HeaderParser *hp = new HeaderParser(rawHeader);
    setHeader(
              WizData::LEtoUint32(hp->versionParsed), //Parse header version 4-byte LE
              new valtype(hp->prevHashParsed), //Parse previos block hash 32-byte LE
              new valtype(hp->merkeRootParsed), //Parse merkle root 32-byte LE
              WizData::LEtoUint32(hp->timestampParsed), //Parse timestamp 4-byte LE
              WizData::LEtoUint32(hp->bitsParsed), //Parse bits 4-byte LE
              WizData::LEtoUint32(hp->nonceParsed), //Parse nonce 4-byte LE
              blockHash
              );
    delete hp;
}

// We can construct header from 6 components
Header::Header(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce) {
    HeaderConstructor *hc = new HeaderConstructor(version, prevHash, merkeRoot, timestamp, bits, nonce);
    valtype *blockHash = new valtype(32);
    CSHA256().Write(hc->rawHeader.data(), hc->rawHeader.size()).Finalize((*blockHash).data());
    CSHA256().Write((*blockHash).data(), (*blockHash).size()).Finalize((*blockHash).data());
    delete hc;
    setHeader(new uint32_t(version),new valtype(prevHash),new valtype(merkeRoot),new uint32_t(timestamp),new uint32_t(bits),new uint32_t(nonce),blockHash);
}

void Header::setHeader(uint32_t *version, valtype *prevHash, valtype *merkeRoot, uint32_t *timestamp, uint32_t *bits, uint32_t *nonce, valtype *blockHash){
    bool suc = true;
    //Check 1
    if (syncHeight != -1)
        if(getHeaderHash(syncHeight) != *prevHash)
            suc = false;
    if(suc == true) {
        this->hash = *blockHash;
        this->height = ++syncHeight;
        this->version = *version;
        this->prevHash = *prevHash;
        this->merkeRoot = *merkeRoot;
        this-> timestamp = *timestamp;
        this->bits = *bits;
        this-> nonce = *nonce;
    }
    delete version; delete prevHash; delete merkeRoot; delete timestamp; delete bits; delete nonce; delete blockHash;
}

uint32_t Header::getHeaderVersion(uint64_t height) {
    return ((Header*)Header::headerAddresses[height])->version;
}

valtype Header::getHeaderPrevHash(uint64_t height) {
    return ((Header*)Header::headerAddresses[height])->prevHash;
}

valtype Header::getHeaderMerkeRoot(uint64_t height) {
    return ((Header*)Header::headerAddresses[height])->merkeRoot;
}

uint32_t Header::getHeaderTimestamp(uint64_t height) {
    return ((Header*)Header::headerAddresses[height])->timestamp;
}

uint32_t Header::getHeaderBits(uint64_t height) {
    return ((Header*)Header::headerAddresses[height])->bits;
}

uint32_t Header::getHeaderNonce(uint64_t height) {
    return ((Header*)Header::headerAddresses[height])->nonce;
}

valtype Header::getHeaderHash(uint64_t height) {
    return ((Header*)Header::headerAddresses[height])->hash;
}

uint32_t Header::getHeaderHeight(uint64_t height) {
    return ((Header*)Header::headerAddresses[height])->height;
}
