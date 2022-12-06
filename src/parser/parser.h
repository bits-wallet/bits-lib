//
//  headerParser.hpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#ifndef headerParser_h
#define headerParser_h

#include <vector>
#include <iostream>
#include "../util/wizdata.h"

typedef std::vector<unsigned char> valtype;

class Parser {
};

class HeaderParser: public Parser {
public:
    valtype versionParsed;
    valtype prevHashParsed;
    valtype merkeRootParsed;
    valtype timestampParsed;
    valtype bitsParsed;
    valtype nonceParsed;
public:
    HeaderParser(valtype rawHeader);
};

class HeaderConstructor: public Parser {
public:
    valtype rawHeader;
public:
    HeaderConstructor(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce);
};


#include <stdio.h>

#endif /* headerParser_h */
