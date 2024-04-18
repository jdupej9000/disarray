#include "encoding.h"

namespace dsry::encoding
{

    static const uint8_t DivideBy7Rev_64[65] = { 9,9,8,8,8,8,8,8,8,7,7,7,7,7,7,7,6,6,6,6,6,6,6,5,5,5,5,5,5,5,4,4,4,4,4,4,4,3,3,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,0 };
    static const uint8_t DivideBy7Rev_32[33] = { 4,4,4,4,4,3,3,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,0,0,0,0,0,0,0 };
    static const uint32_t UpperBit_7b8b_32[5] = { 0x0, 0x80, 0x8080, 0x808080, 0x80808080 };
    static const uint64_t UpperBit_7b8b_64[10] = { 0x0, 0x80, 0x8080, 0x808080, 0x80808080, 0x8080808080, 0x808080808080, 0x80808080808080, 0x8080808080808080, 0x8080808080808080 };

    constexpr uint32_t Mask_7b8b_32 = 0b01111111011111110111111101111111u;
    constexpr uint64_t Mask_7b8b_64 = 0b0111111101111111011111110111111101111111011111110111111101111111llu;
    constexpr uint32_t Mask_InterleaveOdd_32 = 0b01010101010101010101010101010101;
    constexpr uint64_t Mask_InterleaveOdd_64 = 0b0101010101010101010101010101010101010101010101010101010101010101;


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

    int decode_leb128_bmi(uint8_t* pSrc, uint32_t& x)
    {
        uint64_t src = *(const uint64_t*)pSrc;
        uint64_t maskBoundary = ~(src | Mask_7b8b_64);
        uint64_t mask = _blsmsk_u64(maskBoundary);
        x = (uint32_t)_pext_u64(src & mask, Mask_7b8b_64);
        return 1 + (_tzcnt_u64(maskBoundary) >> 3);
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

    int decode_leb128_bmi(uint8_t* pSrc, uint64_t& x)
    {
        const uint64_t src = *(const uint64_t*)pSrc;
        const uint64_t maskBoundary = ~(src | Mask_7b8b_64);

        if (maskBoundary != 0)
        {
            const uint64_t mask = _blsmsk_u64(maskBoundary);
            x = _pext_u64(src & mask, Mask_7b8b_64);
            return 1 + (_tzcnt_u64(maskBoundary) >> 3);
        }
        else
        {
            const uint64_t srcHi = *(const uint64_t*)(pSrc + 8);
            const uint64_t maskBoundaryHi = ~(srcHi | Mask_7b8b_64);
            const uint64_t maskHi = _blsmsk_u64(maskBoundaryHi);
            x = _pext_u64(src, Mask_7b8b_64) | (_pext_u64(srcHi & maskHi, Mask_7b8b_64) << 56);
            return 9 + (_tzcnt_u64(maskBoundaryHi) >> 3);
        }
    }

    uint32_t encode_morton_bmi(uint16_t a, uint16_t b)
    {
        return _pdep_u32(a, Mask_InterleaveOdd_32) | _pdep_u32(b, ~Mask_InterleaveOdd_32);
    }

    void decode_morton_bmi(uint32_t x, uint16_t& a, uint16_t& b)
    {
        a = _pext_u32(x, Mask_InterleaveOdd_32);
        b = _pext_u32(x, ~Mask_InterleaveOdd_32);
    }

    uint64_t encode_morton_bmi(uint32_t a, uint32_t b)
    {
        return _pdep_u64(a, Mask_InterleaveOdd_64) | _pdep_u64(b, ~Mask_InterleaveOdd_64);
    }

    void decode_morton_bmi(uint64_t x, uint32_t& a, uint32_t& b)
    {
        a = _pext_u64(x, Mask_InterleaveOdd_64);
        b = _pext_u64(x, ~Mask_InterleaveOdd_64);
    }

    uint32_t encode_zigzag_bmi(int32_t x)
    {
        return (x << 1) ^ (x >> 31);
    }

    int32_t decode_zigzag_bmi(uint32_t x)
    {
        return (x >> 1) ^ (-(int32_t)(x & 1));
    }

    uint64_t encode_zigzag_bmi(int64_t x)
    {
        return (x << 1) ^ (x >> 63);
    }

    int64_t decode_zigzag_bmi(uint64_t x)
    {
        return (x >> 1) ^ (-(int64_t)(x & 1));
    }
};