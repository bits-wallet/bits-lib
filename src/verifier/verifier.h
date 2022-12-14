//
//  verifier.h
//  Bits
//
//  Created by Burak on 17.12.2022.
//

#ifndef verifier_h
#define verifier_h

#include <stdio.h>
#include "../util/wizdata.h"
#include "../crypto/sha256.h"
#include "../transaction/transaction.h"
#include "../utxo/utxo.h"
#include "../utreexo.h"
#include "../block/block.h"
#include "../utxo/extract.h"


class VerifierSync {
public:
    static uint32_t syncHeight;
    static std::vector<UTXO> coinbaseUTXOs;
    static utreexo::Pollard forestState;
    static std::array<unsigned char, 1024> getRoots();
    static uint64_t getNumRoots();
    static uint64_t getNumLeaves();
    static uint32_t getSyncHeight();
    static uint32_t returnCoinbaseUTXOIndex(valtype prevHash, uint32_t vout);
    static uint32_t returnCoinbaseUTXOIndex(valtype prevHash);
    static uint32_t getCoinbaseUTXOsSize();
    static std::array<unsigned char, 5000000> getCoinbaseUTXOs();
    
public:
    VerifierSync();
    VerifierSync(uint32_t startHeight, uint64_t numLeaves, uint64_t numRoots, std::array<unsigned char, 1024>roots, uint32_t numCoinbaseUTXOs, std::array<unsigned char, 5000000> coinbaseUTXOs);
};

class Verifier {
public:
    Verifier() {};
    bool verify(valtype rawBlock, valtype spendings, valtype proof);
private:
    int returnCollectionIndex(valtype prevHash, uint32_t vout);
    std::vector<uint32_t> cbSpentIndexes;
    utreexo::BatchProof batchProof;
    std::vector<Hash> spentHashes;
    std::vector<Bytes> txIDs;
    std::vector<UTXO*> OutCollection;
    uint64_t *inputSats = 0;
    uint64_t *outputSats = 0;
    uint32_t getThisHeight() { return VerifierSync::syncHeight + 1; }
};

#endif /* verifier_h */
