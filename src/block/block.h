//
//  block.hpp
//  Bits
//
//  Created by Burak on 8.12.2022.
//

#ifndef block_h
#define block_h

#include <stdio.h>
#include "../util/wizdata.h"
#include "../crypto/sha256.h"
#include "../transaction/transaction.h"
#include "../header.h"

class Block {
public:
    std::vector<Transaction> transactions;
    Block() {};
    void setBlock(std::vector<Transaction> transactions) { this->transactions = transactions; };
    static Block submitNewBlock(valtype vRawBlock, uint32_t height);
};

#endif /* block_h */
