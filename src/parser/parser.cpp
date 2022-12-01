//
//  headerParser.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#include "parser.h"

HeaderParser::HeaderParser(valtype rawHeader){
    std::cout << "hp ini " << std::endl;
    this->versionParsed.insert(versionParsed.begin(), rawHeader.begin(), rawHeader.begin()+4);
    this->prevHashParsed.insert(prevHashParsed.begin(), rawHeader.begin()+4, rawHeader.begin()+36);
    this->merkeRootParsed.insert(merkeRootParsed.begin(), rawHeader.begin()+36, rawHeader.begin()+68);
    this->timestampParsed.insert(timestampParsed.begin(), rawHeader.begin()+68, rawHeader.begin()+72);
    this->bitsParsed.insert(bitsParsed.begin(), rawHeader.begin()+72, rawHeader.begin()+76);
    this->nonceParsed.insert(nonceParsed.begin(), rawHeader.begin()+76, rawHeader.begin()+80);
}
