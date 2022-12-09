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
    Hash returnLeafHash();
};

#endif /* utxo_h */
