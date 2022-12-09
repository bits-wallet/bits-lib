//
//  utxo.cpp
//  Bits
//
//  Created by Burak on 8.12.2022.
//

#include "utxo.h"

Hash UTXO::returnLeafHash() {
    valtype rawLeaf; valtype *hashLeaf = new valtype(32);
    valtype *vHeight = WizData::Uint32ToLE(this->height);
    valtype *vVout = WizData::Uint32ToLE(this->vout);
    valtype *vValue = WizData::Uint64ToLE(this->value);
    
    rawLeaf.insert(rawLeaf.begin(), vHeight->begin(), vHeight->end());
    rawLeaf.insert(rawLeaf.end(), this->prevHash.begin(), this->prevHash.end());
    rawLeaf.insert(rawLeaf.end(), vVout->begin(), vVout->end());
    rawLeaf.insert(rawLeaf.end(), vValue->begin(), vValue->end());
    rawLeaf.insert(rawLeaf.end(), this->scriptPubkey.begin(), this->scriptPubkey.end());

    CSHA256().Write(rawLeaf.data(), rawLeaf.size()).Finalize(hashLeaf->data());
    
    Hash hash = {};

    for(int i = 0; i < 32; i++){
        hash[i] = (uint8_t)(*hashLeaf)[i];
    }

    delete hashLeaf;
    return hash;
}
