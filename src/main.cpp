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

    valtype rawtx = stringToValtype("010000000842544d55e5ca36281cc1dfc24f47f4af4cf8cd672e728639dcd0b81fe31d99e700000000fdfd0000473044022011fd24839d4bb215a1a873a6e55423de1cdb92d905d9bb11e0f564b5ec4d107a02200b9e93f3a810438b248f56ce90478163afbbf99d074d40dcf988e2fceff1841a01483045022100eaac8e7f5a59f34ba0f3d7622c5ce4d6783fad854ca57b5d403f3a4d455c4e0a022029c3ffaba0095e768c3b6223a5b9790d45ace56e9d8d01f44c05556e92c31b84014c695221032afb63ccee47e1e207403a9a4de224e592ea4bb72c0194057d9e29f4068dfebf2102ab7d82e42c137eae4dd27e1db7b92ba379f2a6e2dd0777116bfbf145e8d3be7721025bd9707e3f87dbec43166541903802ffd58c476354bdb83d659072f71e9cfb7c53aeffffffff0049931d8cd7805f377c796a5ec7713f968613e4d55a73960c0e8c1634f4575600000000fdfd0000483045022100c7d098b5bea421a988f753f8e8f81f4203a349dc14fdd1f53bf8e02bb444cf5002207b0c049fe30c890562e4335ab7a10da2799f710f35fc14786d77f4b8a594b7c201473044022031ae87605518cc06ae19ac9cec7d9284c14496efa315e2e51d7add1361e291c0022020bd43fd12739227ca63e117c6e87d1ceb637ea70565188e1beab82f5df995cf014c695221021ddcafadf7deae4bff4200c3051c896ea62274568eb41ccf519d41ade3a0092921033f2a088a0bdd4f92a809afb6650602dad862ef77b650f2408a5a99a1f68457ce210276b9019fa41b7997a38c431379deea006b4d3a8004e14b102526c0598622428653aefffffffff3b0d019f179362ebd8044efe10befaf242d8125b2d3667f227f0a36ea77972500000000fc0047304402205dcbb68b1a78cd0fb710ea4683fc53d82a0e808a15a134a5c2246605b74a177f0220521754cf89eb5c2ba6ac541eca40457dc75b4c5a48fb12febb3345166d52a24d01473044022046644210a2cba4d2230009096db10b30092896e2c1461d062cdaaa04df26eed802204476d7af3651b3c7152e11eafc2b04a2d39b8d5ae531925cf9476b6c9c97e784014c69522103a06f99638828cfb441478b54876df5dcf6f518021b5632c4f12d899a89e6d31c21022239e29de023cf4fd1812b0589a5edb0e725f7caef7e6bab9717d65a93412e40210287be21a5b79f839020dbd33f7035ac2be9b66a8a81506f61ee14d774a833d75b53aeffffffff7a5a3f93e2d6b656447c50a70c21ee556869be53b5f862190e6205cf413048a301000000fc0047304402207d10133cff923b8c2f1f2011c00d8ae86b6247a64b51458d026ae7b15ead562902203180cc7eb1ed2ca0684da371da99539a4bec8a415d838ee3ef960e5860c386c101473044022029d004bc6619f6f1457008d7985d539988ca5ea5ac1dd3632c0a5dec6f12421e0220479a31e4f025bc3b91a814779fb9fcce8385e769928b7ec02be1107a16f89dd9014c69522102d653e37d6de8a1a671b5833a1f4d5e2597c9532e05fe9c5b2796da7c46e373a12103d1074e43711292adc65f54d1cb45b86216088e3b0853c2cde78b6c8fb36a7a8b21026e4b44b487b129a3ca6e0d796fb122cf3266990f91f322713444b8b5c536420b53aeffffffff5b5fc3eb77a492c6309c7a51970bc67017b67995258ca0a9f7dacf18b0dcf81200000000fdfd0000483045022100ed375a5ab9e35147e1cab7c5845e52eb8782332cf55456c1c4ce4e0c1f0cb6310220590f72aee139b6e689f44c33b0f63137f542489077c56a27b61a31796c3a21120147304402202a8d12ad677b7913be94f84ec0dfc5f6cc930ef8db1d07360fe48f0266ae884f02207309601d0ab8b5e5c838f56a27169d940932b003a4bfb332512fa9eec90b37bf014c6952210307d87c7b2b4ced8d5268c560b7a3797f26eaf53bb39c892a9659d0b31b4f7a062102a43543638874bb60890ecaa0b96c7a2f8b91e3bc7bbe23d1b0ecb7106ae9d6b92103c15b8183286f0d0dde47caf12d74d5205be4b91083c365514dd18dd04cf13f7853aeffffffff2ce68ccf5e051251d2efe6a59286018d971976b204e44622af3ee81d65227e1b00000000fc0047304402207af8ab131ff0bbde7e6acc749bd0036fcbd327ec2e5725c1c89d01dd6353905702202cfa4093e6d5382ea64d2d35bbdd07526cd2ce3da5d82b6046012224da0df40601473044022071dffa4b15236e528b7890991b9e67305848ce28dc25a23ec2f4d4764cfb3570022055892eca59591755997456a3d7d92ab249d13488f6e2e7184a70ebf0b0a0b0e9014c69522102ad39a7be52c70ec465c2bbce51e2f42e33838aaeafd93b6370716fb800b9b40521025412d72cfab29373ed97df22804560e8ed602c5c22eae1d5349175094345b5022103076c69eadf673b283d5ae57e4a7c28b6f2bb720f34111f46cc1d2312e50a25c853aeffffffff67085330c32517198c2319b4f6f48b6df21a1301cdffb9f4e0e5ef633be096ad01000000fdfe000048304502210088df28030a1dfb1b1226999725385fec2345b6216b7f68472cd8aedc80c60eba02201c091524e969b073d3047026f4712066533fc081de144bcfc5d00834dacbda5001483045022100ae0dfd23084135fff2da31547b287a2a1ce77e42d7abad10f364796f025b2b1f02205b20f027c330268ce20306ca6825de5e14e5992a8d2be3153aebfbae72468a2a014c695221025341f5ca4e6aaee851f0d01df8f931f0d1745434dfa049544ef6c46c750156d02103c3eec96e5156190d0d0a99f6345cb7851856d870fdb3358a8ffd9ea6ee172d1921027ceb63afd79f68ca47dc7e6d0350126c55a6e4f6c8120b23d5445425b34ac06c53aeffffffffa3ea6dc961e781794cd410ef937ecef345fc6bdce0e8114851b2a7435b47f3db00000000fdfd00004730440220308f490dd2a33fed632697a935eec585e4a7a607002b9ba38fcdb6cd88f788070220040d558f47c103456e4c4f8587d1075d4c18e402c1a7a9778ee7d28e0401870d01483045022100928dca77f2cd8481b7d8b567ab9311373bd439d4283b45b0354db3a98b2023f1022072df933ae4ea0aa92dd9ff0dd61201d21ab066231870863c778aedef738c035c014c69522102d83223d297e31baf027bc4914552f1e9abc789ed5348dd234bab23f2bcbccc192102a7eca4a4da05063e306720978883a6dc0128efdaaa373bd1cd933c1c3797fbc62102b49429a18a7c4481be13db03641c6efa20cd2f836b0487c893cfe944ee6cd36753aeffffffff090d610200000000001976a9143c92dd0f56327947e0ef389d048820e17131f74a88ac47918a0300000000160014abaabc5ed3b793d9ac8087f247ae24b2f2df2109c4275a0000000000160014562d1f7cb0b3430cad7ea2e8b629799bad685246ce0b12000000000017a91494b73e1f14040e448373f7b68108791af5271682879ec2fa05000000001600142c6a946bdfbbd6916bf6ab5d735534fb40881268dfd659000000000017a914e661b8ee8eab444e2f51d3125d7f574cdf0055ac87418b0c000000000017a914ce6f14f65792f01847e082dbf64a6fbeb845fed287809698000000000017a914a3c7aa0f2f63cee99ff67b34a6491f5cd3fd289987e3bc630000000000220020e5c7c00d174631d2d1e365d6347b016fb87b6a0c08902d8e443989cb771fa7ec00000000");

    std::cout << "rawtx size: " << rawtx.size() << std::endl;
    
    Transaction tx(rawtx);
    
    std::cout << "version: " << tx.version << std::endl;
   
    std::cout << "inputs size: " << tx.inputs.size() << std::endl;
    std::cout << "outputs size: " << tx.outputs.size() << std::endl;
    std::cout << "locktime: " << tx.locktime << std::endl;
    
    std::cout << "txid: " << (int)tx.txid[0] << std::endl;
    std::cout << "wtxid: " << (int)tx.wtxid[0] << std::endl;
    
    std::string s;
    std::cin >> s;
    
return 0;
}
