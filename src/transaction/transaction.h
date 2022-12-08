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
#include "../crypto/sha256.h"


class TxIn {
public:
    valtype prevOutHash;
    uint32_t voutIndex;
    valtype scriptSig;
    uint32_t sequence;
    std::vector<valtype> witness;
    TxIn(valtype prevOutHash, uint32_t voutIndex, valtype scriptSig, uint32_t sequence): prevOutHash(prevOutHash), voutIndex(voutIndex), scriptSig(scriptSig), sequence(sequence) {}
};

class TxOut {
public:
    uint64_t amount;
    valtype scriptPubkey;
    TxOut(uint64_t amount, valtype scriptPubkey): amount(amount), scriptPubkey(scriptPubkey) {}
};

class Transaction {
public:
    valtype txid;
    valtype wtxid;
    u_int32_t version;
    std::vector<TxIn> inputs;
    std::vector<TxOut> outputs;
    u_int32_t locktime;
public:
    Transaction(valtype txid, valtype wtxid, u_int32_t version, std::vector<TxIn> inputs, std::vector<TxOut> outputs, u_int32_t locktime);
};

#endif /* transaction_h */

