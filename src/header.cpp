//
//  header.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#include "header.h"
#include "parser/parser.h"

std::vector<uint64_t> Header::headerAddresses;
uint32_t HeaderSync::startingSyncHeight;
uint32_t HeaderSync::syncHeight;

HeaderSync::HeaderSync(){
    syncHeight = -1;
    startingSyncHeight = 0;
    Header *genesisHeader = new Header(Hardcoded::genesisVersion,
                                   Hardcoded::genesisPrevHeaderHash(),
                                   Hardcoded::genesisMerkleRootHash(),
                                   Hardcoded::genesisTimestamp,
                                   Hardcoded::genesisBits,
                                   Hardcoded::genesisNonce);
    Header::headerAddresses.push_back((uint64_t)genesisHeader);
};

HeaderSync::HeaderSync(uint32_t startingHeight, valtype rawHeader){
    syncHeight = -1;
    startingSyncHeight = 0;
    valtype *blockHash = new valtype(32);
    CSHA256().Write(rawHeader.data(), rawHeader.size()).Finalize((*blockHash).data());
    CSHA256().Write((*blockHash).data(), (*blockHash).size()).Finalize((*blockHash).data());
    HeaderParser *hp = new HeaderParser(rawHeader);
    Header *sh = new Header(*WizData::LEtoUint32(hp->versionParsed), //Parse header version 4-byte LE
              (hp->prevHashParsed), //Parse previos block hash 32-byte LE
              (hp->merkeRootParsed), //Parse merkle root 32-byte LE
              *WizData::LEtoUint32(hp->timestampParsed), //Parse timestamp 4-byte LE
              *WizData::LEtoUint32(hp->bitsParsed), //Parse bits 4-byte LE
              *WizData::LEtoUint32(hp->nonceParsed) //Parse nonce 4-byte LE
              );
    Header::headerAddresses.push_back((uint64_t)sh);
    syncHeight = startingHeight;
    startingSyncHeight = startingHeight;
    delete hp;
}

HeaderSync::HeaderSync(uint32_t startingHeight, uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce) {
    syncHeight = -1;
    startingSyncHeight = 0;
    HeaderConstructor *hc = new HeaderConstructor(version, prevHash, merkeRoot, timestamp, bits, nonce);
    valtype *blockHash = new valtype(32);
    CSHA256().Write(hc->rawHeader.data(), hc->rawHeader.size()).Finalize((*blockHash).data());
    CSHA256().Write((*blockHash).data(), (*blockHash).size()).Finalize((*blockHash).data());
    Header *sh = new Header(version, prevHash, merkeRoot, timestamp, bits, nonce);
    Header::headerAddresses.push_back((uint64_t)sh);
    syncHeight = startingHeight;
    startingSyncHeight = startingHeight;
    delete hc;
}

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
    if (HeaderSync::syncHeight != -1)
        if(getHeaderHash(HeaderSync::syncHeight) != *prevHash)
            suc = false;
    if(suc == true) {
        this->hash = *blockHash;
        this->height = ++HeaderSync::syncHeight;
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
    return ((Header*)Header::headerAddresses[height - HeaderSync::startingSyncHeight])->version;
}

valtype Header::getHeaderPrevHash(uint64_t height) {
    return ((Header*)Header::headerAddresses[height - HeaderSync::startingSyncHeight])->prevHash;
}

valtype Header::getHeaderMerkeRoot(uint64_t height) {
    return ((Header*)Header::headerAddresses[height - HeaderSync::startingSyncHeight])->merkeRoot;
}

uint32_t Header::getHeaderTimestamp(uint64_t height) {
    return ((Header*)Header::headerAddresses[height - HeaderSync::startingSyncHeight])->timestamp;
}

uint32_t Header::getHeaderBits(uint64_t height) {
    return ((Header*)Header::headerAddresses[height - HeaderSync::startingSyncHeight])->bits;
}

uint32_t Header::getHeaderNonce(uint64_t height) {
    return ((Header*)Header::headerAddresses[height - HeaderSync::startingSyncHeight])->nonce;
}

valtype Header::getHeaderHash(uint64_t height) {
    return ((Header*)Header::headerAddresses[height - HeaderSync::startingSyncHeight])->hash;
}

uint32_t Header::getHeaderHeight(uint64_t height) {
    return ((Header*)Header::headerAddresses[height - HeaderSync::startingSyncHeight])->height;
}

