#include "encoding.h"
#include <intrin.h>

static const uint8_t DivideBy7Rev_64[65] = { 9,9,8,8,8,8,8,8,8,7,7,7,7,7,7,7,6,6,6,6,6,6,6,5,5,5,5,5,5,5,4,4,4,4,4,4,4,3,3,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,0 };
static const uint8_t DivideBy7Rev_32[33] = { 4,4,4,4,4,3,3,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,0 };
static const uint32_t UpperBit_7b8b_32[5] = { 0x0, 0x80, 0x8080, 0x808080, 0x80808080 };
static const uint64_t UpperBit_7b8b_64[10] = { 0x0, 0x80, 0x8080, 0x808080, 0x80808080, 0x8080808080, 0x808080808080, 0x80808080808080, 0x8080808080808080, 0x8080808080808080 };

constexpr uint32_t Mask_7b8b_32 = 0b01111111011111110111111101111111u;
constexpr uint64_t Mask_7b8b_64 = 0b0111111101111111011111110111111101111111011111110111111101111111llu;
constexpr uint32_t Mask_InterleaveOdd_32 = 0b01010101010101010101010101010101;
constexpr uint32_t Mask_InterleaveOdd_64 = 0b0101010101010101010101010101010101010101010101010101010101010101;

int encode_leb128_bmi(uint8_t* pDest, uint32_t x)
{
    int length = DivideBy7Rev_32[_lzcnt_u32(x)];

    // We may write more Bytes than needed, but the pointer will be
    // incremented by the correct amount. So we need the buffer to be
    // at least 7 Bytes extra at the end for safety.
    uint64_t* pDest64 = (uint64_t*)pDest;
    *pDest64 = _pdep_u64(x, Mask_7b8b_64) | UpperBit_7b8b_64[length];

    return length + 1;
}

int encode_leb128_bmi(uint8_t* pDest, uint64_t x)
{
    int length = DivideBy7Rev_64[_lzcnt_u64(x)];

    // We may write more Bytes than needed, but the pointer will be
    // incremented by the correct amount. So we need the buffer to be
    // at least 7 Bytes extra at the end for safety.
    uint64_t* pDest64 = (uint64_t*)pDest;
    pDest64[0] = _pdep_u64(x, Mask_7b8b_64) | UpperBit_7b8b_64[length];

    if (length >= 8)
        pDest64[1] = _pdep_u64(x >> 56, Mask_7b8b_64) | UpperBit_7b8b_64[length - 8];

    return length + 1;
}

uint32_t encode_morton_bmi(uint16_t a, uint16_t b)
{
    return _pdep_u32(a, Mask_InterleaveOdd_32) | _pdep_u32(b, ~Mask_InterleaveOdd_32);
}

uint64_t encode_morton_bmi(uint32_t a, uint32_t b)
{
    return _pdep_u64(a, Mask_InterleaveOdd_64) | _pdep_u64(b, ~Mask_InterleaveOdd_64);
}

uint32_t encode_zigzag_bmi(int32_t x)
{
    int32_t mask = x >> 31; // we need arithmetic shift here (MSVC is OK)
    return _rorx_u32(x, 31) ^ (mask << 1);
    //return (x >= 0) ? (x << 1) : ((-x) << 1) | 0x1;
}

uint64_t encode_zigzag_bmi(int64_t x)
{
    int64_t mask = x >> 63; // we need arithmetic shift here (MSVC is OK)
    return _rorx_u64(x, 63) ^ (mask << 1);
}