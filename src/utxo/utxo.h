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

using Hash = std::array<uint8_t, 32>;
using Leaf = std::pair<Hash, bool>;

class UTXO {
public:
    valtype height;
    valtype prevHash;
    valtype vout;
    valtype value;
    valtype scriptPubkey;
    UTXO(valtype prevHash, valtype vout, valtype height, valtype value, valtype scriptPubkey): prevHash(prevHash), vout(vout), height(height), value(value), scriptPubkey(scriptPubkey) {}
    Hash returnLeafHash();
};

#endif /* utxo_h */
