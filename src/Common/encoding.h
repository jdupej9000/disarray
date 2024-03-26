#pragma once

#include "config.h"
#include <stdint.h>
#include <intrin.h>

namespace dsry::encoding
{

	// Encode u64 or u32 using LEB128 into pDest and return the number of Bytes added.
	int encode_leb128_bmi(uint8_t* pDest, uint64_t x);
	int encode_leb128_bmi(uint8_t* pDest, uint32_t x);
	int decode_leb128_bmi(uint8_t* pSrc, uint64_t& x);
	int decode_leb128_bmi(uint8_t* pSrc, uint32_t& x);

	// Interleave bits of a and b. (b[n-1] a[n-1] ... b[1] a[1] b[0] a[0])
	uint32_t encode_morton_bmi(uint16_t a, uint16_t b);
	uint64_t encode_morton_bmi(uint32_t a, uint32_t b);
	void decode_morton_bmi(uint32_t x, uint16_t& a, uint16_t& b);
	void decode_morton_bmi(uint64_t x, uint32_t& a, uint32_t& b);

	// Encode signed integer as unsigned uniquely. (0,-1,1,-2) -> (0,1,2,3)
	uint32_t encode_zigzag_bmi(int32_t x);
	uint64_t encode_zigzag_bmi(int64_t x);
	int32_t decode_zigzag_bmi(uint32_t x);
	int64_t decode_zigzag_bmi(uint64_t x);

	// Perform exp-Golomb encoding of x with M=2^N. The encoded value is returned
	// and number of bits used stored in bits. If bits>64 the function has failed
	// and the result is undefined. UNTESTED
	template<uint32_t N>
	uint64_t encode_expgolomb(uint32_t x, int& bits)
	{
		uint32_t q = x >> N;
		uint64_t r = x & ((1 << N) - 1);
		bits = q + 1 + N;
		return ((1ull << q) - 1) | (r << (q + 1));
	}

	template<uint32_t N>
	uint32_t decode_expgolomb_bmi(uint64_t x, int& bits)
	{
		uint32_t q = _tzcnt_u64(~x);
		uint32_t r = (x >> (q + 1)) & ((1 << N) - 1);
		bits = q + 1 + N;
		return (((uint64_t)q) << N) | r;
	}

};

#if defined(DSRY_BMI)

#define encode_leb128 encode_leb128_bmi
#define decode_leb128 decode_leb128_bmi
#define encode_morton encode_morton_bmi
#define decode_morton decode_morton_bmi
#define encode_zigzag encode_zigzag_bmi
#define decode_zigzag decode_zigzag_bmi
#define decode_expgolomb decode_expgolomb_bmi

#else

#pragma message ("encoding.h uses BMI despite DSRY_BMI being not defined.")
#define encode_leb128 encode_leb128_bmi
#define decode_leb128 decode_leb128_bmi
#define encode_morton encode_morton_bmi
#define decode_morton decode_morton_bmi
#define encode_zigzag encode_zigzag_bmi
#define decode_zigzag decode_zigzag_bmi
#define decode_expgolomb decode_expgolomb_bmi

#endif