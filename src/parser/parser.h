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

#include <stdio.h>

#endif /* headerParser_h */
