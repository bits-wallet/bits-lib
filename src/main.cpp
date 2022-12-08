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
#include "block/block.h"
#include "prover/prover.h"

using namespace std::chrono;

valtype stringToValtype(std::string const& hex) {
    
    std::string newStr = "";
    
    for (int i = 0; i < (hex.size()/2); i++) {
        newStr += hex.substr((i*2),2) + " ";
    }
    
    std::string cipher = newStr;
    
    
    std::istringstream strm{cipher};
    strm >> std::hex;

    return {std::istream_iterator<int>{strm}, {}};
}


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
    
    valtype rawHeader = stringToValtype("010000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d6190000000000982051fd1e4ba744bbbe680e1fee14677ba1a3c3540bf7b1cdb606e857233e0e61bc6649ffff001d01e36299");

    Header *newHeader = new Header(rawHeader);
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

void submitRawBlock(valtype vRawBlock, uint32_t height){
    Block block(vRawBlock, height);
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

    valtype rawblock = stringToValtype("010000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d6190000000000982051fd1e4ba744bbbe680e1fee14677ba1a3c3540bf7b1cdb606e857233e0e61bc6649ffff001d01e36299018601000000010000000000000000000000000000000000000000000000000000000000000000ffffffff0704ffff001d0104ffffffff0100f2052a0100000043410496b538e853519c726a2c91e61ec11600ae1390813a627c66fb8be7947be63c52da7589379515d4e0a604f8141781e62294721166bf621e73a82cbf2342c858eeac00000000");
    
    //Prover(rawblock, 1);
    
    ProverSync *ps = new ProverSync;
    Prover(rawblock, 1);
    
    
    std::string s;
    std::cin >> s;
    
return 0;
}

