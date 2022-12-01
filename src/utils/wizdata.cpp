//
//  wizdata.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#include "wizdata.h"

uint32_t WizData::LEtoUint32(valtype in){
    uint32_t val = 0;
    for (int i = 0; i < in.size(); i++) { val+= (int)in[i] * pow(256,i); }
    return val;
}

uint64_t WizData::LEtoUint64(valtype in){
    uint64_t val = 0;
    for (int i = 0; i < in.size(); i++) { val+= (int)in[i] * pow(256, i); }
    return val;
}

valtype WizData::hexStringToValtype(std::string in){
    
    std::stringstream ss;

    for (int i=0; i < in.size()/2; i++) {

        std::stringstream icat;
        icat << in[i*2] << in[i*2+1];
        std::string icatstr = icat.str();
        unsigned char hexNumber;
        sscanf(icatstr.c_str(), "%x", &hexNumber);
        ss << hexNumber;
    }
    
    const unsigned char* t = reinterpret_cast<const unsigned char*>(ss.str().c_str());
    valtype returnVal;
    
    for (int i=0; i < in.size()/2; i++){
        returnVal.push_back(*(t+i));
    }
    
    return  returnVal;
}
