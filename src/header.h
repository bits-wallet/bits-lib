//
//  header.hpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#ifndef header_h
#define header_h

#include <stdio.h>
#include "utils/wizdata.h"
#include "crypto/sha256.h"
#include "hardcoded.h"



class Header {
private: //Sync height
    void setHeader(uint32_t *version, valtype *prevHash, valtype *merkeRoot, uint32_t *timestamp, uint32_t *bits, uint32_t *nonce, valtype *blockHash);

public: //Header info
    uint32_t height;
    valtype hash;
    
private: //Header components
    uint32_t version;
    valtype prevHash;
    valtype merkeRoot;
    uint32_t timestamp;
    uint32_t bits;
    uint32_t nonce;
    
public: //Header constructers
    Header(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce);
    Header(valtype rawHeader);
    
public:
    static std::vector<uint64_t> headerAddresses;
    static valtype getHeaderPrevHash(uint64_t height);
    static valtype getHeaderMerkeRoot(uint64_t height);
    static valtype getHeaderHash(uint64_t height);
    static uint32_t getHeaderVersion(uint64_t height);
    static uint32_t getHeaderTimestamp(uint64_t height);
    static uint32_t getHeaderBits(uint64_t height);
    static uint32_t getHeaderNonce(uint64_t height);
    static uint32_t getHeaderHeight(uint64_t height);
};

class HeaderSync {
public:
    static uint32_t periodBeginningBits;
    static uint32_t periodBeginningTimestamp;
    static uint32_t startingSyncHeight;
    static uint32_t syncHeight;
    static uint32_t getSyncHeight() { return syncHeight; };
    HeaderSync();
    HeaderSync(uint32_t startingHeight, uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce, uint32_t periodBeginningBits, uint32_t periodBeginningTimestamp);
    HeaderSync(uint32_t startingHeight, valtype rawHeader, uint32_t periodBeginningBits, uint32_t periodBeginningTimestamp);
};

#endif /* header_h */
