//
//  main.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//
#include "header.h"
#include "arith_uint256.h"

uint32_t getCurrentHeaderSyncHeight() {
    return HeaderSync::getSyncHeight();
}

bool submitRawHeader(valtype rawHeader){
    uint32_t preSyncHeight = HeaderSync::getSyncHeight();
    Header *newHeader = new Header(rawHeader);
    Header::headerAddresses.push_back((uint64_t)newHeader);
    uint32_t postSyncHeight = HeaderSync::getSyncHeight();
    return (preSyncHeight != postSyncHeight);
}

bool submitHeaderFromComponents(uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce){
    uint32_t preSyncHeight = HeaderSync::getSyncHeight();
    Header *newHeader = new Header(version, prevHash, merkeRoot, timestamp, bits, nonce);
    Header::headerAddresses.push_back((uint64_t)newHeader);
    uint32_t postSyncHeight = HeaderSync::getSyncHeight();
    //std::cout << "reee "<< (preSyncHeight != postSyncHeight) << std::endl;
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
    std::cout << "submit: " << submitHeaderFromComponents(1, prevHash, merkeRoot, 1231469665, 486604799, 2573394689) << std::endl;
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
    
    std::cout << "submit: " << submitHeaderFromComponents(1, prevHash, merkeRoot, 1231469744, 486604799, 1639830024) << std::endl;
}

void test_submit_header_3() {
    valtype x1 = WizData::hexStringToValtype("bddd99ccfda39da1b108ce1a5d7003");
    valtype x2 = WizData::hexStringToValtype("8d0a967bacb68b6b63065f626a00000000");
    valtype x3 = WizData::hexStringToValtype("44f672226090d85db9a9f2fbfe5f0f96");
    valtype x4 = WizData::hexStringToValtype("09b387af7be5b7fbb7a1767c831c9e99");
    
    valtype prevHash;
    prevHash.insert(prevHash.begin(), x1.begin(),x1.end());
    prevHash.insert(prevHash.begin() + x1.size() , x2.begin(),x2.end());
    valtype merkeRoot;
    merkeRoot.insert(merkeRoot.begin(), x3.begin(),x3.end());
    merkeRoot.insert(merkeRoot.begin() + x3.size() , x4.begin(),x4.end());
    
    std::cout << "submit: " << submitHeaderFromComponents(1, prevHash, merkeRoot, 1231470173, 486604799, 1844305925) << std::endl;
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
    std::cout << "sdf: " << std::endl;
}

void initHeaderSyncGenesis() {
    HeaderSync();
}

void initHeaderSyncFromHeight(uint32_t startHeight, uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce) {
    HeaderSync(startHeight, version, prevHash, merkeRoot, timestamp, bits, nonce);
}

int main() {
    
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
    
    //initHeaderSyncFromHeight(2, 1, prevHash, merkeRoot, 1231469744, 486604799, 1639830024);
    initHeaderSyncGenesis();
    
    std::cout <<  HeaderSync::getSyncHeight()  << std::endl;
    
    test_submit_header_1();
    test_submit_header_2();
    test_submit_header_3();

    std::cout <<  HeaderSync::getSyncHeight()  << std::endl;
    //std::cout <<  Header::getHeaderTimestamp(3)  << std::endl;

    
    std::string s;
    std::cin >> s;
    
return 0;
}


