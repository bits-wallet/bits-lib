//
//  transaction.hpp
//  Bits
//
//  Created by Burak on 6.12.2022.
//

#ifndef transaction_hp
#define transaction_h

#include <stdio.h>
#include "../util/wizdata.h"



class TxIn {
private:
    valtype prevOutHash;
    uint32_t voutIndex;
    valtype scriptSig;
    uint32_t sequence;
};

class TxOut {
private:
    uint64_t amount;
    valtype scriptPubkey;
};

class WitSlot {
private:
    std::vector<valtype> elements;
};

class Transaction {
private:
    bool isWitnessSerialization;
    u_int32_t version;
    std::vector<TxIn> inputs;
    std::vector<TxOut> outputs;
    std::vector<WitSlot> witness;
    u_int32_t locktime;
};

#endif /* transaction_h */
