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
    std::vector<UTXO> utxos;
    std::vector<uint8_t> serializedProof;
    Proof() {};
    bool importUTXOs (valtype rawImport);
    bool importUTXOsPartial (valtype *rawImport, std::vector<valtype> *prevouts, std::vector<uint32_t> *vouts, uint64_t *inputSats);
    valtype exportUTXOs (std::vector<UTXO> utxos);
    std::vector<Hash> returnUTXOHashes();
    std::vector<Leaf> returnUTXOLeaves();
};

#endif /* proof_h */
