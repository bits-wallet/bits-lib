//
//  wizdata.hpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#ifndef wizdata_h
#define wizdata_h

#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <sstream>
#include "../utils/common.h"
#include "../uint256.h"

typedef std::vector<unsigned char> valtype;

class WizData {
public:
    static valtype buffer32ToValtype(unsigned char *buffer);
    static valtype buffer80ToValtype(unsigned char *buffer);
    static uint256 *LEtoUint256(valtype in);
    static valtype *Uint32ToLE(uint32_t in);
    static uint32_t *LEtoUint32(valtype in);
    static uint64_t *LEtoUint64(valtype in);
    static valtype hexStringToValtype(std::string in);
};

#endif /* wizdata_h */
