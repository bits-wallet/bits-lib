//
//  proof.hpp
//  Bits
//
//  Created by Burak on 9.12.2022.
//

#ifndef proof_h
#define proof_h

#include <stdio.h>
#include "utxo.h"

class Proof {
public:
    static valtype exportUTXOsRaw (std::vector<UTXO> utxos);
public:
    std::vector<UTXO> utxos;
    std::vector<uint8_t> serializedProof;
    Proof() {};
    bool importUTXOs (valtype rawImport);
    std::vector<Hash> returnUTXOHashes();
};

#endif /* proof_h */
