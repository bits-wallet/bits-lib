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

class Header {
    
private: //Sync height
    static uint32_t syncHeight;
    void setHeader(uint32_t *version, valtype *prevHash, valtype *merkeRoot, uint32_t *timestamp, uint32_t *bits, uint32_t *nonce, valtype *blockHash);

private: //Header info
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
    void getInfo();
};

#endif /* header_h */
