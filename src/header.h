//
//  header.hpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//

#ifndef header_hpp
#define header_hpp

#include <stdio.h>
#include <vector>

class Header {
private:
    uint32_t version;
    std::vector<unsigned char> prevHash;
    std::vector<unsigned char> merkeRoot;
    uint32_t timestamp;
    uint32_t bits;
    uint32_t nonce;
};

#endif /* header_hpp */
