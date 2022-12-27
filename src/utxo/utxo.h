//
//  utxo.hpp
//  Bits
//
//  Created by Burak on 8.12.2022.
//

#ifndef utxo_h
#define utxo_h

#include <stdio.h>
#include "../util/wizdata.h"
#include "../crypto/sha256.h"
#include <array>

class UTXO {
public:
    uint32_t height;
    valtype prevHash;
    uint32_t vout;
    uint64_t value;
    valtype scriptPubkey;
    UTXO(uint32_t height, valtype prevHash, uint32_t vout, uint64_t value, valtype scriptPubkey): height(height), prevHash(prevHash), vout(vout), value(value), scriptPubkey(scriptPubkey) {}
    ~UTXO() { std::cout << "deleted utxo: "<< this<< std::endl; };
    Hash returnLeafHash();
};

class PartialUTXO {
public:
    uint32_t height;
    uint64_t value;
    valtype scriptPubkey;
    PartialUTXO(uint32_t height, uint64_t value, valtype scriptPubkey): height(height), value(value), scriptPubkey(scriptPubkey) {}
    ~PartialUTXO() { std::cout << "deleted PartialUTXO utxo: "<< this<< std::endl; };
};

#endif /* utxo_h */
