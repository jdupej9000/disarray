#pragma once

#include <stdint.h>

// Encode u64 or u32 using LEB128 into pDest and return the number of Bytes added.
int encode_leb128_bmi(uint8_t* pDest, uint64_t x);
int encode_leb128_bmi(uint8_t* pDest, uint32_t x);

// Interleave bits of a and b. (b[n-1] a[n-1] ... b[1] a[1] b[0] a[0])
uint32_t encode_morton_bmi(uint16_t a, uint16_t b);
uint64_t encode_morton_bmi(uint32_t a, uint32_t b);

// Encode signed integer as unsigned uniquely. (0,-1,1,-2) -> (0,1,2,3)
uint32_t encode_zigzag_bmi(int32_t x);
uint64_t encode_zigzag_bmi(int64_t x);

// 
int decode_leb128_bmi(uint8_t* pSrc, uint64_t& x);
int decode_leb128_bmi(uint8_t* pSrc, uint32_t& x);

// 
void decode_morton_bmi(uint32_t x, uint16_t& a, uint16_t& b);
void decode_morton_bmi(uint64_t x, uint32_t& a, uint32_t& b);

// 
int32_t decode_zigzag_bmi(uint32_t x);
int64_t decode_zigzag_bmi(uint64_t x);
