#include "RomixCpu.hpp"

RomixCpu::RomixCpu()
{    
    // allocate scratch buffers for single threaded romix version on cpu
    mV.resize(mBlockSize * mCostFactor);
    mXY.resize(256 * mBlockSizeFactor);
}

/**
Execute romix function in place on array of input data.
 @param data Array of bytes to mix.
 @param blocks Number of blocks in input data, each block must be mBlockSize bytes.
 */
void RomixCpu::mix(uint8_t* data, size_t blocks)
{
    for(size_t i = 0 ; i < blocks; ++i)
    {
        smix(&(data[i * mBlockSize]), mBlockSizeFactor, mCostFactor, mV.data(), mXY.data());
    }
}

/**
 Bulk copy from source to destination array.
 @param dest Destination byte array.
 @param src Source byte array.
 @param len Number of contiguous bytes to copy.
 */
void RomixCpu::blkcpy(uint8_t* dest, uint8_t* src, size_t len)
{
    size_t i;

    for (i = 0; i < len; i++)
        dest[i] = src[i];
}

/**
 Bulk exlusive or between two byte arrays.
 @param dest Input byte array one, and destination byte array for xor.
 @param src Inpute byte array two.
 @param len Number of contiguous bytes to xor and save.
 */
void RomixCpu::blkxor(uint8_t* dest, uint8_t* src, size_t len)
{
    size_t i;

    for (i = 0; i < len; i++)
        dest[i] ^= src[i];
}

/**
 Apply the salsa20/8 algorithum to the provided block.
 See https://en.wikipedia.org/wiki/Salsa20
 @param B A 64-byte chuck of data to hash with salsa20/8 Input byte array one, and destination byte array for xor.
 */
void RomixCpu::salsa20_8(uint8_t B[64])
{
    uint32_t B32[16];
    uint32_t x[16];
    size_t i;

    /* Convert little-endian values in. */
    for (i = 0; i < 16; i++)
        B32[i] = RomixCpu::le32dec(&B[i * 4]);

    /* Compute x = doubleround^4(B32). */
    for (i = 0; i < 16; i++)
        x[i] = B32[i];
    for (i = 0; i < 8; i += 2) {
#define R(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
        /* Operate on columns. */
        x[ 4] ^= R(x[ 0]+x[12], 7);  x[ 8] ^= R(x[ 4]+x[ 0], 9);
        x[12] ^= R(x[ 8]+x[ 4],13);  x[ 0] ^= R(x[12]+x[ 8],18);

        x[ 9] ^= R(x[ 5]+x[ 1], 7);  x[13] ^= R(x[ 9]+x[ 5], 9);
        x[ 1] ^= R(x[13]+x[ 9],13);  x[ 5] ^= R(x[ 1]+x[13],18);

        x[14] ^= R(x[10]+x[ 6], 7);  x[ 2] ^= R(x[14]+x[10], 9);
        x[ 6] ^= R(x[ 2]+x[14],13);  x[10] ^= R(x[ 6]+x[ 2],18);

        x[ 3] ^= R(x[15]+x[11], 7);  x[ 7] ^= R(x[ 3]+x[15], 9);
        x[11] ^= R(x[ 7]+x[ 3],13);  x[15] ^= R(x[11]+x[ 7],18);

        /* Operate on rows. */
        x[ 1] ^= R(x[ 0]+x[ 3], 7);  x[ 2] ^= R(x[ 1]+x[ 0], 9);
        x[ 3] ^= R(x[ 2]+x[ 1],13);  x[ 0] ^= R(x[ 3]+x[ 2],18);

        x[ 6] ^= R(x[ 5]+x[ 4], 7);  x[ 7] ^= R(x[ 6]+x[ 5], 9);
        x[ 4] ^= R(x[ 7]+x[ 6],13);  x[ 5] ^= R(x[ 4]+x[ 7],18);

        x[11] ^= R(x[10]+x[ 9], 7);  x[ 8] ^= R(x[11]+x[10], 9);
        x[ 9] ^= R(x[ 8]+x[11],13);  x[10] ^= R(x[ 9]+x[ 8],18);

        x[12] ^= R(x[15]+x[14], 7);  x[13] ^= R(x[12]+x[15], 9);
        x[14] ^= R(x[13]+x[12],13);  x[15] ^= R(x[14]+x[13],18);
#undef R
    }

    /* Compute B32 = B32 + x. */
    for (i = 0; i < 16; i++)
        B32[i] += x[i];

    /* Convert little-endian values out. */
    for (i = 0; i < 16; i++)
        RomixCpu::le32enc(&B[4 * i], B32[i]);
}


/**
 Decode 32-bit chunck of data as if its little endian formated.
 @param pp A 32-bit chuck of data.
 @return Little endian encoded 32bit unsigned integer.
 */
inline uint32_t RomixCpu::le32dec(const void* pp)
{
    const uint8_t* p = (uint8_t const*)pp;
    return ((uint32_t)(p[0]) + ((uint32_t)(p[1]) << 8) +
            ((uint32_t)(p[2]) << 16) + ((uint32_t)(p[3]) << 24));
}

/**
 Encode 32-bit chunck of data from a little endian formated 32-bit unsigned it.
 @param pp Output 32-bit chuck of data.
 @param x Input 32-bit unsigned int.
 */
inline void RomixCpu::le32enc(void* pp, uint32_t x)
{
    uint8_t* p = (uint8_t*)pp;
    p[0] = x & 0xff;
    p[1] = (x >> 8) & 0xff;
    p[2] = (x >> 16) & 0xff;
    p[3] = (x >> 24) & 0xff;
}

/**
 Decode 64-bit chunck of data as if its little endian formated.
 @param pp A 64-bit chuck of data.
 @return Little endian encoded 64-bit unsigned integer.
 */
inline uint64_t RomixCpu::le64dec(const void* pp)
{
    const uint8_t* p = (uint8_t const*)pp;

    return ((uint64_t)(p[0]) + ((uint64_t)(p[1]) << 8) +
            ((uint64_t)(p[2]) << 16) + ((uint64_t)(p[3]) << 24) +
            ((uint64_t)(p[4]) << 32) + ((uint64_t)(p[5]) << 40) +
            ((uint64_t)(p[6]) << 48) + ((uint64_t)(p[7]) << 56));
}


/**
 * blockmix_salsa8(B, Y, r):
 * Compute B = BlockMix_{salsa20/8, r}(B).  The input B must be 128r bytes in
 * length; the temporary space Y must also be the same size.
 */

/**
 Execute the blockmix algorithum with salsa20/8 hash on single mBlockSize block of data.
 See https://en.wikipedia.org/wiki/Scrypt
 @param B mBlockSize input data byte array representing a single block.
 @param r Number of 128 bytes chunks in a block, equal to mBlockSizeFactor.
 */
void RomixCpu::blockmix_salsa8(uint8_t* B, uint8_t* Y, size_t r)
{
    uint8_t X[64];
    size_t i;

    /* 1: X <-- B_{2r - 1} */
    RomixCpu::blkcpy(X, &B[(2 * r - 1) * 64], 64);

    /* 2: for i = 0 to 2r - 1 do */
    for (i = 0; i < 2 * r; i++) {
        /* 3: X <-- H(X \xor B_i) */
        RomixCpu::blkxor(X, &B[i * 64], 64);
        RomixCpu::salsa20_8(X);

        /* 4: Y_i <-- X */
        RomixCpu::blkcpy(&Y[i * 64], X, 64);
    }

    /* 6: B' <-- (Y_0, Y_2 ... Y_{2r-2}, Y_1, Y_3 ... Y_{2r-1}) */
    for (i = 0; i < r; i++)
        RomixCpu::blkcpy(&B[i * 64], &Y[(i * 2) * 64], 64);
    for (i = 0; i < r; i++)
        RomixCpu::blkcpy(&B[(i + r) * 64], &Y[(i * 2 + 1) * 64], 64);
}

/**
 Interprete that last 64 bytes of B as a little endian integer.
 See https://en.wikipedia.org/wiki/Scrypt
 @param B mBlockSize block of input data.
 @param r Number of 128 bytes chunks in a block, equal to mBlockSizeFactor.
 @return 64-byte unsigned int.
 */
uint64_t RomixCpu::integerify(uint8_t* B, size_t r)
{
    uint8_t* X = &B[(2 * r - 1) * 64];
    return (RomixCpu::le64dec(X));
}

/**
 * smix(B, r, N, V, XY):
 * Compute B = SMix_r(B, N).  The input B must be 128r bytes in length; the
 * temporary storage V must be 128rN bytes in length; the temporary storage
 * XY must be 256r bytes in length.  The value N must be a power of 2.
 */

/**
 Main romix computation.
 See https://en.wikipedia.org/wiki/Scrypt
 
 @param B Input block to mix,  must be 128*r bytes in length.
 @param r Number of 128 bytes chunks in a block, equal to mBlockSizeFactor.
 @param N Number of iterations (must be a power of 2), equal to mCostFactor.
 @param V Large scratch buffer, ust be 128*r*N bytes in length.
 @param XY Small scratch buffer, 256*r bytes in length.
 */
void RomixCpu::smix(uint8_t* B, size_t r,
    uint64_t N, uint8_t* V, uint8_t* XY)
{
    uint8_t* X = XY;
    uint8_t* Y = &XY[128 * r];
    uint64_t i;
    uint64_t j;

    /* 1: X <-- B */
    RomixCpu::blkcpy(X, B, 128 * r);

    /* 2: for i = 0 to N - 1 do */
    for (i = 0; i < N; i++) {
        /* 3: V_i <-- X */
        RomixCpu::blkcpy(&V[i * (128 * r)], X, 128 * r);

        /* 4: X <-- H(X) */
        RomixCpu::blockmix_salsa8(X, Y, r);
    }

    /* 6: for i = 0 to N - 1 do */
    for (i = 0; i < N; i++) {
        /* 7: j <-- Integerify(X) mod N */
        j = RomixCpu::integerify(X, r) & (N - 1);

        /* 8: X <-- H(X \xor V_j) */
        RomixCpu::blkxor(X, &V[j * (128 * r)], 128 * r);
        RomixCpu::blockmix_salsa8(X, Y, r);
    }

    /* 10: B' <-- X */
    RomixCpu::blkcpy(B, X, 128 * r);
}





