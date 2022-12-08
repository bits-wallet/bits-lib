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
    Block(valtype vRawBlock, uint32_t height);
};

#endif /* block_h */
