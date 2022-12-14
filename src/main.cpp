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

#include "utxo/proof.h"


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

std::string getLastHeight() {
    
    char buf[32];
    std::string query = "curl -H  \"Content-Type: application/json\" -X GET  http://mempool-env.eba-qh6r3f7d.us-east-2.elasticbeanstalk.com/lastblock";
    
    FILE * output = popen(query.c_str(), "r");
    
    while (fgets (buf, 32, output)) {}
    
    pclose(output);
    return buf;
  }

std::string getRawBlock(int height) {
    
    char buf[8000000];
    std::string query = "curl -H  \"Content-Type: application/json\" -X GET  http://mempool-env.eba-qh6r3f7d.us-east-2.elasticbeanstalk.com/block/"+ std::to_string(height) ;
    
    FILE * output = popen(query.c_str(), "r");
    
    while (fgets (buf, 8000000, output)) {}
    
    pclose(output);
    return buf;
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
   
    initHeaderSyncGenesis();
    test_submit_header_1();
    
    //test_submit_header_2();
    //test_submit_header_3();
    
    //valtype rawblock1 = stringToValtype("010000006FE28C0AB6F1B372C1A6A246AE63F74F931E8365E15A089C68D6190000000000982051FD1E4BA744BBBE680E1FEE14677BA1A3C3540BF7B1CDB606E857233E0E61BC6649FFFF001D01E362990101000000010000000000000000000000000000000000000000000000000000000000000000FFFFFFFF0704FFFF001D0104FFFFFFFF0100F2052A0100000043410496B538E853519C726A2C91E61EC11600AE1390813A627C66FB8BE7947BE63C52DA7589379515D4E0A604F8141781E62294721166BF621E73A82CBF2342C858EEAC00000000");
    
    //valtype rawblock2 = stringToValtype("010000004860EB18BF1B1620E37E9490FC8A427514416FD75159AB86688E9A8300000000D5FDCC541E25DE1C7A5ADDEDF24858B8BB665C9F36EF744EE42C316022C90F9BB0BC6649FFFF001D08D2BD610101000000010000000000000000000000000000000000000000000000000000000000000000FFFFFFFF0704FFFF001D010BFFFFFFFF0100F2052A010000004341047211A824F55B505228E4C3D5194C1FCFAA15A456ABDF37F9B9D97A4040AFC073DEE6C89064984F03385237D92167C13E236446B417AB79A0FCAE412AE3316B77AC00000000");
    
    //valtype rawblock3 = stringToValtype("01000000BDDD99CCFDA39DA1B108CE1A5D70038D0A967BACB68B6B63065F626A0000000044F672226090D85DB9A9F2FBFE5F0F9609B387AF7BE5B7FBB7A1767C831C9E995DBE6649FFFF001D05E0ED6D0101000000010000000000000000000000000000000000000000000000000000000000000000FFFFFFFF0704FFFF001D010EFFFFFFFF0100F2052A0100000043410494B9D3E76C5B1629ECF97FFF95D7A4BBDAC87CC26099ADA28066C6FF1EB9191223CD897194A08D0C2726C5747F1DB49E8CF90E75DC3E3550AE9B30086F3CD5AAAC00000000");
    
    
    
    // Prover *block1 = new Prover(rawblock1);
    // delete block1;
    
    // Prover *block2 = new Prover(rawblock2);
    //delete block2;
    
    //Prover *block3 = new Prover(rawblock3);
    
    // std::cout << "proverHeight: " << ProverSync::proverHeight << std::endl;
    
    //std::cout << "size: " << ProverSync::utxoSet.size() << std::endl;
    
    // std::cout << "returnLeafHash: " << (int)ProverSync::utxoSet[0].returnLeafHash()[23] << std::endl;
    
    // valtype bobobo;
    // bobobo = block3->readSpendingsRaw();
    // std::cout << "hohoho: " <<  bobobo.size()  << std::endl;
    
    //Proof xd;
    //std::cout << "hohoho2: " << xd.importUTXOs(bobobo) << std::endl;
    //block3->readProof().Print();
    // std::cout << "hohoho3: "  << std::endl;

    //delete block3;
    //767309
    
    new ProverSync();
    
    for (int l = 1; l < 2147483647; l++) {
        

        
        std::string lastHeightStr = getLastHeight();
        int lastHeight = std::stoi(lastHeightStr.substr(1,lastHeightStr.size()-2));
        std::cout << "lastHeight: " << lastHeight << std::endl;
        
        if(l >= lastHeight){
            break;
        }
        
        std::string aa = getRawBlock(l);
        valtype blockRaw = stringToValtype(aa.substr(1,aa.size()-2));
        Prover *proverBlock = new Prover(blockRaw);
        

        std::vector<uint8_t> proofBlock;
        proverBlock->readProof().Serialize(proofBlock);
        proverBlock->readProof().Print();
  
        std::cout << "proofBlock: " << proofBlock.size() << std::endl;
        
        //std::to_string(l)
        std::string p_str = std::to_string(l) + "_p.bin";
        
        char pn[p_str.size()];
        for (int m = 0; m < p_str.size(); m++) {
            pn[m] = p_str[m];
        }
        FILE * pFile;
        
        char proofBuffer[proofBlock.size()];
                
        for (int z = 0; z < proofBlock.size(); z++) {
            proofBuffer[z] = proofBlock[z];
        }
        
        pFile = fopen ( pn , "w+b" );
        fwrite (proofBuffer , 1 , sizeof(proofBuffer) , pFile );
        fclose (pFile);
        
        
        valtype spendingsBlock = proverBlock->readSpendingsRaw();
        std::cout << "spendingsBlock: " << spendingsBlock.size() << std::endl;
        
        //std::to_string(l)
        std::string s_str = std::to_string(l) + "_s.bin";
        char fn[s_str.size()];
        for (int m = 0; m < s_str.size(); m++) {
            fn[m] = s_str[m];
        }
        FILE * sFile;
        
        char spendingsBuffer[spendingsBlock.size()];
                
        for (int z = 0; z < spendingsBlock.size(); z++) {
            spendingsBuffer[z] = spendingsBlock[z];
        }
        
        sFile = fopen ( fn , "w+b" );
        fwrite (spendingsBuffer , 1 , sizeof(spendingsBuffer) , sFile );
        fclose (sFile);
        
        
        
        
        
        
        delete proverBlock;
        
        
    
        
        
        

        std::cout << "__________PROVER_BLOCK_HEIGHT_" << std::to_string(l) << "_SYNCED_SIZE_" << std::to_string(blockRaw.size()) << "_________"<< std::endl;
        
       
        
    }

    
    
    std::string s;
    std::cin >> s;
    
return 0;
}
