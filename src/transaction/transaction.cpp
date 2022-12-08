//
//  transaction.cpp
//  Bits
//
//  Created by Burak on 6.12.2022.
//

#include "transaction.h"

Transaction::Transaction(valtype txid, valtype wtxid, u_int32_t version, std::vector<TxIn> inputs, std::vector<TxOut> outputs, u_int32_t locktime){
    this->version = version;
    this->inputs = inputs;
    this->outputs = outputs;
    this->locktime = locktime;
    this->txid = txid;
    this->wtxid = wtxid;
}



