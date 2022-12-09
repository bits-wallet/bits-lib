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
    UTXO(valtype prevHash, uint32_t vout, uint32_t height, uint64_t value, valtype scriptPubkey): prevHash(prevHash), vout(vout), height(height), value(value), scriptPubkey(scriptPubkey) {}
    Hash returnLeafHash();
};

#endif /* utxo_h */
