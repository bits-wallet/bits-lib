//
//  hardcoded.h
//  Bits
//
//  Created by Burak on 2.12.2022.
//

#ifndef hardcoded_h
#define hardcoded_h

#include "util/wizdata.h"

class Hardcoded {
public:
    static const uint32_t retargetSeconds = 1209600;
    static const uint32_t genesisVersion = 1;
    static const uint32_t genesisTimestamp = 1231006505;
    static const uint32_t genesisBits = 486604799;
    static const uint32_t genesisNonce = 2083236893;
    
    static const valtype genesisPrevHeaderHash() {
        valtype pZeroPadding32bytes(32);
        return pZeroPadding32bytes;
    };
    
    static const valtype genesisMerkleRootHash() {
        valtype pGenesisMerkeRoot;
        pGenesisMerkeRoot.push_back(0x3b);
        pGenesisMerkeRoot.push_back(0xa3);
        pGenesisMerkeRoot.push_back(0xed);
        pGenesisMerkeRoot.push_back(0xfd);
        pGenesisMerkeRoot.push_back(0x7a);
        pGenesisMerkeRoot.push_back(0x7b);
        pGenesisMerkeRoot.push_back(0x12);
        pGenesisMerkeRoot.push_back(0xb2);
        pGenesisMerkeRoot.push_back(0x7a);
        pGenesisMerkeRoot.push_back(0xc7);
        pGenesisMerkeRoot.push_back(0x2c);
        pGenesisMerkeRoot.push_back(0x3e);
        pGenesisMerkeRoot.push_back(0x67);
        pGenesisMerkeRoot.push_back(0x76);
        pGenesisMerkeRoot.push_back(0x8f);
        pGenesisMerkeRoot.push_back(0x61);
        pGenesisMerkeRoot.push_back(0x7f);
        pGenesisMerkeRoot.push_back(0xc8);
        pGenesisMerkeRoot.push_back(0x1b);
        pGenesisMerkeRoot.push_back(0xc3);
        pGenesisMerkeRoot.push_back(0x88);
        pGenesisMerkeRoot.push_back(0x8a);
        pGenesisMerkeRoot.push_back(0x51);
        pGenesisMerkeRoot.push_back(0x32);
        pGenesisMerkeRoot.push_back(0x3a);
        pGenesisMerkeRoot.push_back(0x9f);
        pGenesisMerkeRoot.push_back(0xb8);
        pGenesisMerkeRoot.push_back(0xaa);
        pGenesisMerkeRoot.push_back(0x4b);
        pGenesisMerkeRoot.push_back(0x1e);
        pGenesisMerkeRoot.push_back(0x5e);
        pGenesisMerkeRoot.push_back(0x4a);
        return pGenesisMerkeRoot;
    };
};

#endif /* hardcoded_h */



