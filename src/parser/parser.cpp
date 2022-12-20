//
//  headerParser.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#include "parser.h"
#include "../crypto/sha256.h"

Bytes returnMerkleRoot(std::vector<Bytes> txids)
{
  if (txids.empty()) {
    throw;
  }

    while (txids.size() > 1) {
        if (txids.size() & 1) {
      txids.push_back(txids.back());
    }
        
    std::vector<Bytes> tmp;
    for (auto it = std::begin(txids); it != std::end(txids) && std::next(it) != txids.end(); it += 2) {
      Bytes concat = *it;
      Bytes result(32);
      
      concat.insert(concat.end(), (*(it + 1)).begin(), (*(it + 1)).end());
      
        CSHA256().Write(concat.data(), concat.size()).Finalize(result.data());
        CSHA256().Write(result.data(), result.size()).Finalize(result.data());
      
      tmp.push_back(result);
      concat.clear();
    }
    txids = tmp;
  }
    return txids[0];
}


HeaderParser::HeaderParser(valtype rawHeader){
    assert(rawHeader.size() == 80);
    this->versionParsed.insert(versionParsed.begin(), rawHeader.begin(), rawHeader.begin()+4);
    this->prevHashParsed.insert(prevHashParsed.begin(), rawHeader.begin()+4, rawHeader.begin()+36);
    this->merkeRootParsed.insert(merkeRootParsed.begin(), rawHeader.begin()+36, rawHeader.begin()+68);
    this->timestampParsed.insert(timestampParsed.begin(), rawHeader.begin()+68, rawHeader.begin()+72);
    this->bitsParsed.insert(bitsParsed.begin(), rawHeader.begin()+72, rawHeader.begin()+76);
    this->nonceParsed.insert(nonceParsed.begin(), rawHeader.begin()+76, rawHeader.begin()+80);
}

HeaderConstructor::HeaderConstructor(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce){
    assert(prevHash.size() == 32 && merkeRoot.size() == 32 && version >= 1);
    
    valtype *pVersion = WizData::Uint32ToLE(version);
    valtype *pTimestamp = WizData::Uint32ToLE(timestamp);
    valtype *pBits = WizData::Uint32ToLE(bits);
    valtype *pNonce = WizData::Uint32ToLE(nonce);
    valtype rawHeader;
    
    rawHeader.insert(rawHeader.begin(), pVersion->begin(), pVersion->end());
    rawHeader.insert(rawHeader.begin()+4, prevHash.begin(), prevHash.end());
    rawHeader.insert(rawHeader.begin()+36, merkeRoot.begin(), merkeRoot.end());
    rawHeader.insert(rawHeader.begin()+68, pTimestamp->begin(), pTimestamp->end());
    rawHeader.insert(rawHeader.begin()+72, pBits->begin(), pBits->end());
    rawHeader.insert(rawHeader.begin()+76, pNonce->begin(), pNonce->end());
    
    this->rawHeader.insert(this->rawHeader.begin(), rawHeader.begin(), rawHeader.end());
    delete pVersion; delete pTimestamp; delete pBits; delete pNonce;
}
