//
//  transaction.hpp
//  Bits
//
//  Created by Burak on 6.12.2022.
//

#ifndef transaction_h
#define transaction_h

#include <stdio.h>
#include "../util/wizdata.h"

class TxIn {
public:
    valtype prevOutHash;
    uint32_t voutIndex;
    valtype scriptSig;
    uint32_t sequence;
    TxIn(valtype prevOutHash, uint32_t voutIndex, valtype scriptSig, uint32_t sequence):
    prevOutHash(prevOutHash), voutIndex(voutIndex), scriptSig(scriptSig), sequence(sequence){}
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
public:
    valtype txid;
    valtype wtxid;
    bool isWitnessSer;
    u_int32_t version;
    std::vector<TxIn> inputs;
    std::vector<TxOut> outputs;
    std::vector<WitSlot> witness;
    u_int32_t locktime;
public:
    Transaction(valtype rawTx);
};

#endif /* transaction_h */

