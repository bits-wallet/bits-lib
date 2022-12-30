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
#include "utxo/extract.h"
#include "verifier/verifier.h"

#include "parser/parser.h"

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
    new HeaderSync();
    return (HeaderSync::ancestorPast11Timestamps.size() == 11);
}

bool initHeaderSyncFromHeightWithComponents(uint32_t startHeight, uint32_t version, valtype prevHash, valtype merkeRoot, uint32_t timestamp, uint32_t bits, uint32_t nonce, uint32_t periodBits, uint32_t periodTimestamp, int atsAr[11]) {
    new HeaderSync(startHeight, version, prevHash, merkeRoot, timestamp, bits, nonce, periodBits, periodTimestamp, atsAr);
    return (HeaderSync::ancestorPast11Timestamps.size() == 11);
}

bool initHeaderSyncFromHeightRaw(uint32_t startHeight, valtype rawHeader, uint32_t periodBits, uint32_t periodTimestamp, int atsAr[11]) {
    new HeaderSync(startHeight, rawHeader, periodBits, periodTimestamp, atsAr);
    return (HeaderSync::ancestorPast11Timestamps.size() == 11);
}

bool initVerifierSync() {
    new VerifierSync();
    return true;
}

bool initVerifierSyncFromHeight(uint32_t startHeight, uint64_t numLeaves, uint64_t numRoots, std::array<unsigned char, 1024>roots) {
    std::array<unsigned char, 5000000> c;
    new VerifierSync(startHeight, numLeaves, numRoots, roots, 0, c);
    return true;
}

std::array<unsigned char, 1024> getForestRoots() {
    return VerifierSync::getRoots();
}

uint64_t getForestNumRoots() {
    return VerifierSync::getNumRoots();
}

uint64_t getForestNumLeaves() {
    return VerifierSync::getNumLeaves();
}

uint32_t getCurrentFullSyncHeight() {
    return VerifierSync::getSyncHeight();
}

uint32_t submitRawBlock(unsigned char rawBlock[], uint32_t rbSize, unsigned char spendings[], uint32_t sSize, unsigned char proof[], uint32_t pSize){
    valtype vRawBlock; valtype vSpendings; std::vector<uint8_t> proofBytes;;
    
    for(int i = 0; i < rbSize; i++) { vRawBlock.push_back(rawBlock[i]); }
    for(int i = 0; i < sSize; i++) { vSpendings.push_back(spendings[i]); }
    for(int i = 0; i < pSize; i++) { proofBytes.push_back((uint8_t)proof[i]); }
    
    Verifier *blockVerifier = new Verifier;
    if (!blockVerifier->verify(vRawBlock, vSpendings, proofBytes)) delete blockVerifier;
    
    return VerifierSync::syncHeight;
}

void test_submit_header_1() {
    
    valtype rawHeader = stringToValtype("010000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d6190000000000982051fd1e4ba744bbbe680e1fee14677ba1a3c3540bf7b1cdb606e857233e0e61bc6649ffff001d01e36299");

    Header *newHeader = new Header(rawHeader);
    bool success = (newHeader->height > 0);
    if (!success) delete newHeader;
    
    std::cout << "submit: " << success << std::endl;
    
}

void test_submit_block_1() {
    
    valtype rawBlock = stringToValtype("010000006FE28C0AB6F1B372C1A6A246AE63F74F931E8365E15A089C68D6190000000000982051FD1E4BA744BBBE680E1FEE14677BA1A3C3540BF7B1CDB606E857233E0E61BC6649FFFF001D01E362990101000000010000000000000000000000000000000000000000000000000000000000000000FFFFFFFF0704FFFF001D0104FFFFFFFF0100F2052A0100000043410496B538E853519C726A2C91E61EC11600AE1390813A627C66FB8BE7947BE63C52DA7589379515D4E0A604F8141781E62294721166BF621E73A82CBF2342C858EEAC00000000");

    valtype vSpendings= stringToValtype("");
    
    std::cout << "vSpendingss: " << vSpendings.size() << std::endl;
    valtype vProof= stringToValtype("0000000000000000");
    std::vector<uint8_t> proofBytes;;
    
    for(int i = 0; i < vProof.size(); i++) { proofBytes.push_back((uint8_t)vProof[i]); }
    
    Verifier *blockVerifier = new Verifier;
    std::cout << "hello: " << blockVerifier->verify(rawBlock, vSpendings, proofBytes) << std::endl;
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

std::string getRawBlock(int height) {
    
    char buf[8000000];
    std::string query = "curl -H  \"Content-Type: application/json\" -X GET  http://mempool-env.eba-qh6r3f7d.us-east-2.elasticbeanstalk.com/block/"+ std::to_string(height) ;
    
    FILE * output = popen(query.c_str(), "r");
    std::cout << "zazab: " << sizeof(*output) << std::endl;
    while (fgets (buf, 8000000, output)) {}
    
    pclose(output);
    return buf;
  }

std::string getLastHeight() {
    
    char buf[32];
    std::string query = "curl -H  \"Content-Type: application/json\" -X GET  http://mempool-env.eba-qh6r3f7d.us-east-2.elasticbeanstalk.com/lastblock";
    
    FILE * output = popen(query.c_str(), "r");
    
    while (fgets (buf, 32, output)) {}
    
    pclose(output);
    return buf;
  }

std::string getBlockS(int height) {
    
    char buf[8000000];
    std::string query = "curl -H  \"Content-Type: application/json\" -X GET  http://18.119.138.90:1923/gets/"+ std::to_string(height) ;
    
    FILE * output = popen(query.c_str(), "r");
    
    while (fgets (buf, 8000000, output)) {}
    
    pclose(output);
        
    return buf;
  }

std::string getBlockP(int height) {
    
    char buf[8000000];
    std::string query = "curl -H  \"Content-Type: application/json\" -X GET  http://18.119.138.90:1923/getp/"+ std::to_string(height) ;
    
    FILE * output = popen(query.c_str(), "r");

    while (fgets (buf, 8000000, output)) {}
    
    pclose(output);
    
    return buf;
  }

int main() {

    initHeaderSyncGenesis();
    //test_submit_header_1();
    //test_submit_header_2();
    //test_submit_header_3();
    
    initVerifierSync();
    //test_submit_block_1();
    
    
    
    for (int l = 1; l < 2147483647; l++) {
        
        std::cout << "CURRENTLY SYNCING: " << l << std::endl;
        
        std::string lastHeightStr = getLastHeight();
        int lastHeight = std::stoi(lastHeightStr.substr(1,lastHeightStr.size()-2));
        std::cout << "lastHeight: " << lastHeight << std::endl;
        
        if(l >= lastHeight){
            break;
        }
        
   
        
        
        
        //BLOCK
        
        std::string newBlock = getRawBlock(l);
        std::string newBlockS = getBlockS(l);
        if(newBlockS == "00")
            newBlockS = "";
        
        std::string newBlockP = getBlockP(l);

        valtype newBlockRaw = stringToValtype(newBlock.substr(1,newBlock.size()-2));
        valtype newBlockPRaw = stringToValtype(newBlockP);
        valtype newBlockSRaw = stringToValtype(newBlockS);
        
        std::cout << "newBlockRaw SAYZ : " << newBlockRaw.size() << std::endl;
        std::cout << "newBlockPRaw SAYZ : " << newBlockPRaw.size() << std::endl;
        std::cout << "newBlockSRaw SAYZ : " << newBlockSRaw.size() << std::endl;
        
        std::vector<uint8_t> proofBytes;;
        for(int i = 0; i < newBlockPRaw.size(); i++) { proofBytes.push_back((uint8_t)newBlockPRaw[i]); }
        
        
        
        //HEADER
        
        valtype rawHeader = stringToValtype(newBlock.substr(1, 160));
        
        std::cout << "rawHeader SAYZ : " << rawHeader.size() << std::endl;

        Header *newHeader = new Header(rawHeader);
        bool success = (newHeader->height > 0);
        if (!success)
            delete newHeader;
        
        assert(success);
        
        std::cout << "HEADERSUB : " << success << std::endl;
        std::cout << "HOAY : " << Header::headerAddresses.size() << std::endl;
        std::cout << "newHeaderheader : " << newHeader->getHeaderMerkeRoot(l).size() << std::endl;
        
        
        Verifier *blockVerifier = new Verifier;
        
        assert(blockVerifier->verify(newBlockRaw, newBlockSRaw, proofBytes));
        
        std::cout << "BLOCKSUB : " << VerifierSync::getSyncHeight() << std::endl;
        
        std::cout << "PR : " << std::endl;
        
        VerifierSync::forestState.PrintRoots();
        
    }
    
    std::string s;
    std::cin >> s;
    
return 0;
}
