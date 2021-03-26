//
//  RomixCpu.hpp
//
//  Created by Braiden Brousseau on 2021-03-26.
//

#ifndef RomixCpu_hpp
#define RomixCpu_hpp

#include <vector>
#include "RomixBase.hpp"

class RomixCpu: public RomixBase
{
public:
    RomixCpu();
    
    void mix(uint8_t* data, size_t blocks);

private:
    
    // scratch buffers
    std::vector<uint8_t> mV;
    std::vector<uint8_t> mXY;
    
    // taken directly from libbitcoin/system/math/external/crypto_scrypt.c
    static void blkcpy(uint8_t*, uint8_t*, size_t);
    static void blkxor(uint8_t*, uint8_t*, size_t);
    static void salsa20_8(uint8_t[64]);
    static void blockmix_salsa8(uint8_t*, uint8_t*, size_t);
    static uint64_t integerify(uint8_t*, size_t);
    static void smix(uint8_t* , size_t, uint64_t, uint8_t*, uint8_t*);
    static inline uint32_t le32dec(const void* pp);
    static inline void le32enc(void* pp, uint32_t x);
    static inline uint64_t le64dec(const void* pp);
};

#endif /* RomixCpu_hpp */
