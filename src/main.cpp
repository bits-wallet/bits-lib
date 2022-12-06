//
//  main.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//
#include "header.h"
#include "arith_uint256.h"
#include <chrono>
#include "transaction/transaction.h"

using namespace std::chrono;

uint32_t getCurrentHeaderSyncHeight() {
    return HeaderSync::getSyncHeight();
}

uint32_t submitRawHeader(unsigned char rawHeader[80]){
    Header *newHeader = new Header(WizData::buffer80ToValtype(rawHeader));
    bool success = (newHeader->height > 0);
    if (!success) delete newHeader;
    return HeaderSync::syncHeight;
}

uint32_t submitHeaderFromComponents(uint32_t version, unsigned char prevHash[32], unsigned char merkeRoot[32], uint32_t timestamp, uint32_t bits, uint32_t nonce){
    Header *newHeader = new Header(version, WizData::buffer32ToValtype(prevHash), WizData::buffer32ToValtype(merkeRoot), timestamp, bits, nonce);
    bool success = (newHeader->height > 0);
    if (!success) delete newHeader;
    return HeaderSync::syncHeight;
}

bool initHeaderSyncGenesis() {
    HeaderSync();
    return (HeaderSync::ancestorPast11Timestamps.size() == 11);
}

bool initHeaderSyncFromHeightWithComponents(uint32_t startHeight, uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce, uint32_t periodBits, uint32_t periodTimestamp, int atsAr[11]) {
    HeaderSync(startHeight, version, prevHash, merkeRoot, timestamp, bits, nonce, periodBits, periodTimestamp, atsAr);
    return (HeaderSync::ancestorPast11Timestamps.size() == 11);
}

bool initHeaderSyncFromHeightRaw(uint32_t startHeight, valtype rawHeader, uint32_t periodBits, uint32_t periodTimestamp, int atsAr[11]) {
    HeaderSync(startHeight, rawHeader, periodBits, periodTimestamp, atsAr);
    return (HeaderSync::ancestorPast11Timestamps.size() == 11);
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

    Header *newHeader = new Header(1, prevHash, merkeRoot, 1231469665, 486604799, 2573394689);
    bool success = (newHeader->height > 0);
    if (!success) delete newHeader;
    
    std::cout << "submit: " << success << std::endl;
    
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
    
    Header *newHeader = new Header(1, prevHash, merkeRoot, 1231469744, 486604799, 1639830024);
    bool success = (newHeader->height > 0);
    if (!success) delete newHeader;
    
    std::cout << "submit: " << success << std::endl;
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
    
    Header *newHeader = new Header(1, prevHash, merkeRoot, 1231470173, 486604799, 1844305925);
    bool success = (newHeader->height > 0);
    if (!success) delete newHeader;
    
    std::cout << "submit: " << success << std::endl;
    }

int main() {
    
    //valtype x1 = WizData::hexStringToValtype("4860eb18bf1b1620e37e9490fc8a427514");
    //valtype x2 = WizData::hexStringToValtype("416fd75159ab86688e9a8300000000");
    //valtype x3 = WizData::hexStringToValtype("d5fdcc541e25de1c7a5addedf24858b8");
    //valtype x4 = WizData::hexStringToValtype("bb665c9f36ef744ee42c316022c90f9b");
    
    //valtype prevHash;
    //prevHash.insert(prevHash.begin(), x1.begin(),x1.end());
    //prevHash.insert(prevHash.begin() + x1.size() , x2.begin(),x2.end());
    // valtype merkeRoot;
    //merkeRoot.insert(merkeRoot.begin(), x3.begin(),x3.end());
    //merkeRoot.insert(merkeRoot.begin() + x3.size() , x4.begin(),x4.end());
    
    //int tsar[] = {0,0,0,0,0,0,0,0,0,0,0};
    
    //initHeaderSyncFromHeightWithComponents(2, 1, prevHash, merkeRoot, 1231469744, 486604799, 1639830024, 486604799, 1231006505, tsar);
   
    //initHeaderSyncGenesis();
    
    //test_submit_header_1();
    //test_submit_header_2();
    //test_submit_header_3();
    
    valtype x1 = WizData::hexStringToValtype("01000000010000000000000000000000");
    valtype x2 = WizData::hexStringToValtype("00000000000000000000000000000000");
    valtype x3 = WizData::hexStringToValtype("0000000000ffffffff4d04ffff001d01");
    valtype x4 = WizData::hexStringToValtype("04455468652054696d65732030332f4a");
    valtype x5 = WizData::hexStringToValtype("616e2f32303039204368616e63656c6c");
    valtype x6 = WizData::hexStringToValtype("6f72206f6e206272696e6b206f662073");
    valtype x7 = WizData::hexStringToValtype("65636f6e64206261696c6f757420666f");
    valtype x8 = WizData::hexStringToValtype("722062616e6b73ffffffff0100f2052a");
    valtype x9 = WizData::hexStringToValtype("01000000434104678afdb0fe55482719");
    valtype x10 = WizData::hexStringToValtype("67f1a67130b7105cd6a828e03909a679");
    valtype x11 = WizData::hexStringToValtype("62e0ea1f61deb649f6bc3f4cef38c4f3");
    valtype x12 = WizData::hexStringToValtype("5504e51ec112de5c384df7ba0b8d578a");
    valtype x13 = WizData::hexStringToValtype("4c702b6bf11d5fac00000000");
    
    valtype rawtx;
    rawtx.insert(rawtx.begin(), x1.begin(),x1.end());
    rawtx.insert(rawtx.end(), x2.begin(),x2.end());
    rawtx.insert(rawtx.end(), x3.begin(),x3.end());
    rawtx.insert(rawtx.end(), x4.begin(),x4.end());
    rawtx.insert(rawtx.end(), x5.begin(),x5.end());
    rawtx.insert(rawtx.end(), x6.begin(),x6.end());
    rawtx.insert(rawtx.end(), x7.begin(),x7.end());
    rawtx.insert(rawtx.end(), x8.begin(),x8.end());
    rawtx.insert(rawtx.end(), x9.begin(),x9.end());
    rawtx.insert(rawtx.end(), x10.begin(),x10.end());
    rawtx.insert(rawtx.end(), x11.begin(),x11.end());
    rawtx.insert(rawtx.end(), x12.begin(),x12.end());
    rawtx.insert(rawtx.end(), x13.begin(),x13.end());
    
    Transaction tx(rawtx);
    
    std::cout << tx.version << std::endl;
    std::cout << tx.inputs[0].voutIndex << std::endl;
    std::cout << tx.inputs[0].sequence << std::endl;
    std::cout << tx.inputs.size() << std::endl;
    std::cout << tx.inputs[0].scriptSig.size() << std::endl;
    
    std::cout << (int)(tx.inputs[0].prevOutHash[0]) << std::endl;
    std::cout << (int)(tx.inputs[0].prevOutHash[1]) << std::endl;
    std::cout << (int)(tx.inputs[0].prevOutHash[2]) << std::endl;
    std::cout << (int)(tx.inputs[0].prevOutHash[30]) << std::endl;
    std::cout << (int)(tx.inputs[0].prevOutHash[31]) << std::endl;
        
    std::cout << (int)(tx.inputs[0].prevOutHash.size()) << std::endl;
    std::string s;
    std::cin >> s;
    
return 0;
}


