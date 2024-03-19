#pragma once

#include "bench.h"

struct schraudolph_exp
{
	static std::string get_name()
	{
		return "schraudolph_exp";
	}

	static __m256 test(__m256 x) noexcept
	{
		/* max. rel. error = 3.55959567e-2 on [-87.33654, 88.72283] */
		__m256 a = _mm256_set1_ps(12102203.0f); /* (1 << 23) / log(2) */
		__m256i b = _mm256_set1_epi32(127 * (1 << 23) - 298765);
		__m256i t = _mm256_add_epi32(_mm256_cvtps_epi32(_mm256_mul_ps(a, x)), b);
		return _mm256_castsi256_ps(t);
	}
};