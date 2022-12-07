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

valtype stringToValtype(std::string const& hex) {
    
    std::string newStr = "";
    
    for (int i = 0; i < (hex.size()/2); i++) {
        newStr += hex.substr((i*2),2) + " ";
    }
    
    std::cout << "newStr: " << newStr << std::endl;
    
    std::string cipher = newStr;
    
    
    std::istringstream strm{cipher};
    strm >> std::hex;

    return {std::istream_iterator<int>{strm}, {}};
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
    

    valtype rawtx = stringToValtype("01000000000103d6b2fd4aa08561fc5ac2bac3f98e659d4dee4348d951bc9f4020807c9699640f000000006a473044022047d794bd53a535f92ca319506f3ad8c3cb4b5f264d3678d7c49819f6a33545d202202f93978659eeeabd6a012fa66767ef546d83e8925c5cbf127d11c2ff15ca34b9012102cdee8333c31afd53bb47a2485d19547de52737309c9ec20ff8de47e17e681f32ffffffff64639a7c7cdd19922bd1204b60672ea7d23e0a31d5a20e2f370cc4330120eb320000000000ffffffff198027c781754935d9ed9d40bb6e36a35a5a1d7bdcb542375ddb0ec43b724a5f0000000000ffffffff02814c0000000000001600146eb836821861fbb534c19e9026f9613f6be2dd1e220202000000000017a914bd799d6e1ae8280d6ce762b4669780b75d323f6987000247304402200f6f4de73bf214a33eeb00bd88601f199ccb4d6d67d7c35a37198bcce5738f11022004bcec7c5a046052359821dcbb6da1b28d8ea3c957cf75ce4fd159cb706fbfe50121025217ed98be4666c35c3a19ad17e7dc81d006576f4381a3cfc4621b41dbb623e9024730440220462831c7cc525f5c0ea5fef156f7a00f2409f899b66af13c4868e7d5ddbd372202207e9fac0b5bfcbb8c69d17f2157e5dd7fa053a8279eb64bbf7c7038fdbfdec9a7012103514042cf576ffd3ac6c8b120a812357b9ce45db3fa06e54113fac534a7a0616e00000000");

    std::cout << rawtx.size() << std::endl;
    
    Transaction tx(rawtx);
    
    std::cout << tx.version << std::endl;
   
    std::cout << (int)tx.inputs[1].witness[0][0] << std::endl;
    std::cout << (int)tx.inputs[1].witness[0][1] << std::endl;
    std::cout << (int)tx.inputs[1].witness[0][2] << std::endl;
    std::cout << (int)tx.inputs[1].witness[0][3] << std::endl;

    std::string s;
    std::cin >> s;
    
return 0;
}


