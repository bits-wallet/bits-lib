//
//  prover.hpp
//  Bits
//
//  Created by Burak on 8.12.2022.
//

#ifndef prover_h
#define prover_h

#include <stdio.h>
#include "../util/wizdata.h"
#include "../crypto/sha256.h"
#include "../transaction/transaction.h"
#include "../header.h"
#include "../utxo/utxo.h"

class ProverSync {
public:
    static std::vector<UTXO> utxoSet;
    static std::vector<Leaf> utxoLeafSet;
    static std::pair<uint32_t, UTXO*> returnUTXOFromOutpoint(valtype prevHash, uint32_t vout);
    static uint32_t proverHeight;
    ProverSync() {};
};

class Prover {
private:
    valtype spendingsRaw;
    std::vector<UTXO> spendings;
    std::vector<Hash> spendingsHashes;
    void setSpendingsRaw();
public:
    Prover(valtype vRawBlock);
    valtype readSpendingsRaw();
    
};

#endif /* prover_h */
