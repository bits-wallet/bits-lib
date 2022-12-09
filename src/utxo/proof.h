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
    //valtype exportUtxosRaw ();
};

#endif /* proof_h */
