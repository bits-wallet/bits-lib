//
//  proof.hpp
//  Bits
//
//  Created by Burak on 9.12.2022.
//

#ifndef extract_h
#define extract_h

#include <stdio.h>
#include "utxo.h"

class Exract {
public:
    Exract() {};
    std::vector<UTXO> UTXOs;
    std::vector<PartialUTXO> partialUTXOs;
    std::vector<uint8_t> serializedProof;
    bool importUTXOs (valtype rawImport);
    bool importUTXOsPartial (valtype *rawImport);
    valtype exportUTXOs (std::vector<UTXO> utxos);
    std::vector<Hash> returnUTXOHashes();
    std::vector<Leaf> returnUTXOLeaves();
    uint32_t elapsed = 0;
};

#endif /* extract_h */
