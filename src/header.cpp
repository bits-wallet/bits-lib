//
//  header.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#include "header.h"
#include "parser/parser.h"

#include "arith_uint256.h"

std::vector<uint64_t> Header::headerAddresses;
uint32_t HeaderSync::startingSyncHeight;
uint32_t HeaderSync::syncHeight;

HeaderSync::HeaderSync(){
    syncHeight = -1; startingSyncHeight = 0;
    new Header(Hardcoded::genesisVersion,
                                   Hardcoded::genesisPrevHeaderHash(),
                                   Hardcoded::genesisMerkleRootHash(),
                                   Hardcoded::genesisTimestamp,
                                   Hardcoded::genesisBits,
                                   Hardcoded::genesisNonce);
};

HeaderSync::HeaderSync(uint32_t startingHeight, valtype rawHeader){
    syncHeight = startingHeight -1; startingSyncHeight = startingHeight;
    valtype *blockHash = new valtype(32);
    CSHA256().Write(rawHeader.data(), rawHeader.size()).Finalize((*blockHash).data());
    CSHA256().Write((*blockHash).data(), (*blockHash).size()).Finalize((*blockHash).data());
    HeaderParser *hp = new HeaderParser(rawHeader);
    new Header(*WizData::LEtoUint32(hp->versionParsed), //Parse header version 4-byte LE
              (hp->prevHashParsed), //Parse previos block hash 32-byte LE
              (hp->merkeRootParsed), //Parse merkle root 32-byte LE
              *WizData::LEtoUint32(hp->timestampParsed), //Parse timestamp 4-byte LE
              *WizData::LEtoUint32(hp->bitsParsed), //Parse bits 4-byte LE
              *WizData::LEtoUint32(hp->nonceParsed) //Parse nonce 4-byte LE
              );
    assert(startingHeight == startingSyncHeight);
    delete hp;
}

HeaderSync::HeaderSync(uint32_t startingHeight, uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce) {
    syncHeight = startingHeight -1; startingSyncHeight = startingHeight;
    HeaderConstructor *hc = new HeaderConstructor(version, prevHash, merkeRoot, timestamp, bits, nonce);
    valtype *blockHash = new valtype(32);
    CSHA256().Write(hc->rawHeader.data(), hc->rawHeader.size()).Finalize((*blockHash).data());
    CSHA256().Write((*blockHash).data(), (*blockHash).size()).Finalize((*blockHash).data());
    new Header(version, prevHash, merkeRoot, timestamp, bits, nonce);
    assert(startingHeight == startingSyncHeight);
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
    bool ovveride = false;
    
    //Check 1
    if (Header::headerAddresses.size() > 0)
        if(getHeaderHash(HeaderSync::syncHeight) != *prevHash) {
            if(getHeaderPrevHash(HeaderSync::syncHeight) == *prevHash){
                //override
                ovveride = true;
            } else {
                //fail
                suc = false;
            }
        }

    //Check 2 target
    arith_uint256 bnTarget;
    bool fNegative; bool fOverflow;
    bnTarget.SetCompact(*bits, &fNegative, &fOverflow);
    if(ArithToUint256(bnTarget).data() < WizData::LEtoUint256(*blockHash)->data())
        suc = false;
    
    //Check 3 retarget
    if (Header::headerAddresses.size() >= 2015)
        if ((HeaderSync::syncHeight + 1) % 2016 == 0) {
            uint32_t secondGap = Header::getHeaderTimestamp(HeaderSync::syncHeight) - Header::getHeaderTimestamp(HeaderSync::syncHeight - 2015);
        
            if (secondGap < Hardcoded::retargetSeconds/4)
                secondGap = Hardcoded::retargetSeconds/4;
            if (secondGap > Hardcoded::retargetSeconds*4)
                secondGap = Hardcoded::retargetSeconds*4;
            
            arith_uint256 bnNew;
                bnNew.SetCompact(Header::getHeaderBits(HeaderSync::getSyncHeight()));
                bnNew *= secondGap;
                bnNew /= Hardcoded::retargetSeconds;
            if(bnNew.GetCompact() != *bits)
                suc = false;
        }
        else {
            if(*bits != Header::getHeaderBits(HeaderSync::getSyncHeight()))
                suc = false;
        }
    
    if(suc == true) {
        this->hash = *blockHash;
        this->version = *version;
        this->prevHash = *prevHash;
        this->merkeRoot = *merkeRoot;
        this-> timestamp = *timestamp;
        this->bits = *bits;
        this-> nonce = *nonce;
        if(ovveride == true) {
            delete (Header*)Header::headerAddresses[Header::headerAddresses.size()-1];
            Header::headerAddresses.erase(Header::headerAddresses.end()-1);
            this->height = HeaderSync::syncHeight;
        } else {
            this->height = ++HeaderSync::syncHeight;
        }
        Header::headerAddresses.push_back((uint64_t)this);
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


