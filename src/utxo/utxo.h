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

class UTXO {
public:
    valtype prevHash;
    valtype vout;
    valtype height;
    valtype value;
    valtype scriptPubkey;
    UTXO(valtype prevHash, valtype vout, valtype height, valtype value, valtype scriptPubkey)
    :prevHash(prevHash), vout(vout), height(height), value(value), scriptPubkey(scriptPubkey) {}
};

#endif /* utxo_h */
