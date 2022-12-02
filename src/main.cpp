//
//  main.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//
#include "header.h"
#include "hardcoded.h"

uint32_t submitRawHeader(valtype rawHeader){
    Header *newHeader = new Header(rawHeader);
    Header::headerAddresses.push_back((uint64_t)newHeader);
    return Header::getSyncHeight();
}

uint32_t submitHeaderFromComponents(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce){
    Header *newHeader = new Header(version, prevHash, merkeRoot, timestamp, bits, nonce);
    Header::headerAddresses.push_back((uint64_t)newHeader);
    return Header::getSyncHeight();
}

void test_2() {
    valtype x1 = WizData::hexStringToValtype("d4d157a3341a9262af2d1d6cce8b8a93");
    valtype x2 = WizData::hexStringToValtype("21660df4bec56ac50a00000000000000");
    
    valtype x3 = WizData::hexStringToValtype("05eb125f08fa288bb0ea9ae08e9fe4b9");
    valtype x4 = WizData::hexStringToValtype("75809431cbb246d10ac18b7f83281dfe");

    valtype prevHash;
    prevHash.insert(prevHash.begin(), x1.begin(),x1.end());
    prevHash.insert(prevHash.begin() + x1.size() , x2.begin(),x2.end());
    
    valtype merkeRoot;
    merkeRoot.insert(merkeRoot.begin(), x3.begin(),x3.end());
    merkeRoot.insert(merkeRoot.begin() + x3.size() , x4.begin(),x4.end());
    
    submitHeaderFromComponents(549453824, prevHash, merkeRoot, 1669898749, 422823818, 2606882314);
}

void submit_genesis_block() {
    submitHeaderFromComponents(Hardcoded::genesisVersion,
                               Hardcoded::genesisPrevHeaderHash(),
                               Hardcoded::genesisMerkleRootHash(),
                               Hardcoded::genesisTimestamp,
                               Hardcoded::genesisBits,
                               Hardcoded::genesisNonce);
}

void test_1() {
    valtype x1 = WizData::hexStringToValtype("0000c020d4d157a3341a9262af2d1d6cce");
    valtype x2 = WizData::hexStringToValtype("8b8a9321660df4bec56ac50a00000000");
    valtype x3 = WizData::hexStringToValtype("00000005eb125f08fa288bb0ea9ae08e9f");
    valtype x4 = WizData::hexStringToValtype("e4b975809431cbb246d10ac18b7f83281d");
    valtype x5 = WizData::hexStringToValtype("fefda188638ac733190ade619b");

    valtype nv;
    nv.insert(nv.begin(), x1.begin(),x1.end());
    nv.insert(nv.begin() + x1.size() , x2.begin(),x2.end());
    nv.insert(nv.begin() + x1.size() + x2.size() , x3.begin(),x3.end());
    nv.insert(nv.begin() + x1.size() + x2.size() + x3.size() , x4.begin(),x4.end());
    nv.insert(nv.begin() + x1.size() + x2.size() + x3.size() + x4.size() , x5.begin(),x5.end());
    
    submitRawHeader(nv);
}

int main() {
    submit_genesis_block();
    
    test_1();
    test_2();
    test_2();
    test_2();
    
    std::cout << (int)Header::getHeaderHash(1)[0] << std::endl;
        
    std::string s;
    std::cin >> s;
    
return 0;
}
