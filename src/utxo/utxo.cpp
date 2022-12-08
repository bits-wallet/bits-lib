//
//  utxo.cpp
//  Bits
//
//  Created by Burak on 8.12.2022.
//

#include "utxo.h"

Hash UTXO::returnLeafHash() {
    valtype rawLeaf; valtype *hashLeaf = new valtype(32);
    rawLeaf.insert(rawLeaf.begin(), this->height.begin(), this->height.end());
    rawLeaf.insert(rawLeaf.end(), this->prevHash.begin(), this->prevHash.end());
    rawLeaf.insert(rawLeaf.end(), this->vout.begin(), this->vout.end());
    rawLeaf.insert(rawLeaf.end(), this->value.begin(), this->value.end());
    rawLeaf.insert(rawLeaf.end(), this->scriptPubkey.begin(), this->scriptPubkey.end());
    
    CSHA256().Write(rawLeaf.data(), rawLeaf.size()).Finalize(hashLeaf->data());
    
    Hash hash = {};

    for(int i = 0; i < 32; i++){
        hash[i] = (uint8_t)(*hashLeaf)[i];
    }

    return hash;
}
