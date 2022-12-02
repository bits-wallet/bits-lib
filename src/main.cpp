//
//  main.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//
#include "header.h"
#include "hardcoded.h"

uint32_t getCurrentHeaderSyncHeight() {
    return Header::getSyncHeight();
}

bool submitRawHeader(valtype rawHeader){
    uint32_t preSyncHeight = Header::getSyncHeight();
    Header *newHeader = new Header(rawHeader);
    Header::headerAddresses.push_back((uint64_t)newHeader);
    uint32_t postSyncHeight = Header::getSyncHeight();
    return (preSyncHeight != postSyncHeight);
}

bool submitHeaderFromComponents(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce){
    uint32_t preSyncHeight = Header::getSyncHeight();
    Header *newHeader = new Header(version, prevHash, merkeRoot, timestamp, bits, nonce);
    Header::headerAddresses.push_back((uint64_t)newHeader);
    uint32_t postSyncHeight = Header::getSyncHeight();
    return (preSyncHeight != postSyncHeight);
}

void test_submit_header_1() {
    valtype x1 = WizData::hexStringToValtype("6fe28c0ab6f1b372c1a6a246ae63f74f");
    valtype x2 = WizData::hexStringToValtype("931e8365e15a089c68d6190000000000");
    valtype x3 = WizData::hexStringToValtype("982051fd1e4ba744bbbe680e1fee1467");
    valtype x4 = WizData::hexStringToValtype("7ba1a3c3540bf7b1cdb606e857233e0e");
    
    valtype prevHash;
    prevHash.insert(prevHash.begin(), x1.begin(),x1.end());
    prevHash.insert(prevHash.begin() + x1.size() , x2.begin(),x2.end());
    valtype merkeRoot;
    merkeRoot.insert(merkeRoot.begin(), x3.begin(),x3.end());
    merkeRoot.insert(merkeRoot.begin() + x3.size() , x4.begin(),x4.end());
    
    std::cout << "test2: " << submitHeaderFromComponents(1, prevHash, merkeRoot, 1231469665, 486604799, 2573394689) << std::endl;
}

void test_submit_header_2() {
    valtype x1 = WizData::hexStringToValtype("4860eb18bf1b1620e37e9490fc8a427514");
    valtype x2 = WizData::hexStringToValtype("416fd75159ab86688e9a8300000000");
    valtype x3 = WizData::hexStringToValtype("d5fdcc541e25de1c7a5addedf24858b8");
    valtype x4 = WizData::hexStringToValtype("bb665c9f36ef744ee42c316022c90f9b");
    
    valtype prevHash;
    prevHash.insert(prevHash.begin(), x1.begin(),x1.end());
    prevHash.insert(prevHash.begin() + x1.size() , x2.begin(),x2.end());
    valtype merkeRoot;
    merkeRoot.insert(merkeRoot.begin(), x3.begin(),x3.end());
    merkeRoot.insert(merkeRoot.begin() + x3.size() , x4.begin(),x4.end());
    
    std::cout << "test2: " << submitHeaderFromComponents(1, prevHash, merkeRoot, 1231469744, 486604799, 1639830024) << std::endl;
}

void submit_genesis_block() {
    std::cout << "test0: " << submitHeaderFromComponents(Hardcoded::genesisVersion,
                               Hardcoded::genesisPrevHeaderHash(),
                               Hardcoded::genesisMerkleRootHash(),
                               Hardcoded::genesisTimestamp,
                               Hardcoded::genesisBits,
                               Hardcoded::genesisNonce)  << std::endl;
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
    
    std::cout << "test1: " << submitRawHeader(nv) << std::endl;
}

int main() {
    
    submit_genesis_block();
    
    test_submit_header_1();
    test_submit_header_2();
    
    std::cout << getCurrentHeaderSyncHeight() << std::endl;
    
    std::cout << (int)Header::getHeaderHash(2)[27] << std::endl;
    
    std::string s;
    std::cin >> s;
    
return 0;
}


