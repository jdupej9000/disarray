#pragma once

#include "bench.h"

struct very_fast_exp
{
	static std::string get_name()
	{
		return "very_fast_exp";
	}

	static __m256 test(__m256 x) noexcept
	{
		const __m256 a = _mm256_set1_ps((1 << 22) / 0.6931472f);  // to get exp(x/2)
		const __m256i b = _mm256_set1_epi32(127 * (1 << 23));       // NB: zero shift!
		__m256i r = _mm256_cvtps_epi32(_mm256_mul_ps(a, x));
		__m256i s = _mm256_add_epi32(b, r);
		__m256i t = _mm256_sub_epi32(b, r);
		return _mm256_div_ps(_mm256_castsi256_ps(s), _mm256_castsi256_ps(t));
	}
};
