//
//  main.cpp
//  Bits
//
//  Created by Burak on 1.12.2022.
//
#include "header.h"






void test_1() {
    
    
    uint32_t x = 4294967294;
    valtype *re = WizData::Uint32ToLE(x);
    
    std::cout << " aka " << (int)(*re)[0] << std::endl;
    std::cout << " aka2 " << (int)(*re)[1] << std::endl;
    std::cout << " aka3 " << (int)(*re)[2] << std::endl;
    std::cout << " aka4 " << (int)(*re)[3] << std::endl;

   
    delete re;
    
    //std::cout << "babazz2 " << (re) << std::endl;
    
  
    valtype x1 = WizData::hexStringToValtype("0000c020d4d157a3341a9262af2d1d6cce");
    valtype x2 = WizData::hexStringToValtype("8b8a9321660df4bec56ac50a00000000");
    valtype x3 = WizData::hexStringToValtype("00000005eb125f08fa288bb0ea9ae08e9f");
    valtype x4 = WizData::hexStringToValtype("e4b975809431cbb246d10ac18b7f83281d");
    valtype x5 = WizData::hexStringToValtype("fefda188638ac733190ade619b");

    valtype nv;
    nv.insert(nv.begin(), x1.begin(),x1.end());
    nv.insert(nv.begin() + x1.size() , x2.begin(),x2.end());
    nv.insert(nv.begin() + x1.size() + x2.size() , x3.begin(),x3.end());
    nv.insert(nv.begin() + x1.size() + x2.size() + x3.size() , x4.begin(),x4.end());
    nv.insert(nv.begin() + x1.size() + x2.size() + x3.size() + x4.size() , x5.begin(),x5.end());
    
    Header *nh = new Header(nv);
    nh->getInfo();
    
    Header *nh2 = new Header(nv);
    nh2->getInfo();
    
    std::cout << "pa: " <<  nh << std::endl;
    std::cout << "pa2: " <<  nh2 << std::endl;


}

int main() {

    test_1();
    
return 0;
}
