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
    static uint32_t syncHeight;
    static utreexo::Pollard forestState;
    static std::array<unsigned char, 1024> getRoots();
    static uint64_t getNumRoots();
    static uint64_t getNumLeaves();
    static uint32_t getSyncHeight();
public:
    VerifierSync(uint32_t startHeight);
    VerifierSync(uint32_t startHeight, uint64_t numLeaves, uint64_t numRoots, std::array<unsigned char, 1024>roots);
    ~VerifierSync() { std::cout << "vs des" << std::endl; }
};

class Verifier {
public:
    Verifier() {};
    bool verify(valtype rawBlock, valtype spendings, valtype proof);
};

#endif /* verifier_h */
