#pragma once
#include <stdint.h>

namespace dsry::math
{
	size_t round_up(size_t x, size_t blk);
	bool is_power_of_two(size_t x);
	uint64_t gcd(uint64_t u, uint64_t v);
	uint64_t lcm(uint64_t u, uint64_t v);

	// Mark the MSB of each field. E.g. to add 8 Bytes, specify 0x8080808080808080.
	template<typename TVal, TVal NMsbMask>
	inline TVal swar_add(TVal a, TVal b) noexcept
	{
		// https://www.chessprogramming.org/SIMD_and_SWAR_Techniques
		return ((a & ~NMsbMask) + (b & ~NMsbMask)) ^ ((a ^ b) & NMsbMask);
	}

	template<typename TVal, TVal NMsbMask>
	inline TVal swar_sub(TVal a, TVal b) noexcept
	{
		return ((a | NMsbMask) - (b & ~NMsbMask)) ^ ((a ^ ~b) & NMsbMask);
	}

	// TODO: https://w3.pppl.gov/~hammett/work/2009/AIM-239-ocr.pdf
};