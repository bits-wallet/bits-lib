//
//  verifier.h
//  Bits
//
//  Created by Burak on 17.12.2022.
//

#ifndef verifier_h
#define verifier_h

#include <stdio.h>
#include "../util/wizdata.h"
#include "../crypto/sha256.h"
#include "../transaction/transaction.h"
#include "../utxo/utxo.h"
#include "../utreexo.h"
#include "../block/block.h"
#include "../utxo/proof.h"

class VerifierSync {
public:
    static utreexo::Pollard forestState;
    static uint32_t syncHeight;
    VerifierSync(uint32_t startHeight) { syncHeight = startHeight; }
};

class Verifier {
public:
    Verifier() {};
    bool verify(valtype rawBlock, valtype spendings, valtype proof);
};

#endif /* verifier_h */
