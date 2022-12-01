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

class Header {
    
private: //Sync height
    static unsigned int syncHeight;
    void setHeader(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce);

private: //Header components
    unsigned int height;
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
