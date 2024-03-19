#pragma once

#include "bench.h"

struct fast_exp
{
	static std::string get_name()
	{
		return "fast_exp";
	}

	static __m256 test(__m256 x) noexcept
	{
		__m256 t, f, p, r;
		__m256i i, j;

		const __m256 l2e = _mm256_set1_ps(1.442695041f); /* log2(e) */
		const __m256 cvt = _mm256_set1_ps(12582912.0f);  /* 1.5 * (1 << 23) */
		const __m256 c0 = _mm256_set1_ps(0.238428936f);
		const __m256 c1 = _mm256_set1_ps(0.703448006f);
		const __m256 c2 = _mm256_set1_ps(1.000443142f);

		/* exp(x) = 2^i * 2^f; i = rint (log2(e) * x), -0.5 <= f <= 0.5 */
		t = _mm256_mul_ps(x, l2e);             /* t = log2(e) * x */
		r = _mm256_sub_ps(_mm256_add_ps(t, cvt), cvt); /* r = rint (t) */
		f = _mm256_sub_ps(t, r);               /* f = t - rint (t) */
		i = _mm256_cvtps_epi32(t);             /* i = (int)t */
		p = c0;                              /* c0 */
		p = _mm256_mul_ps(p, f);               /* c0 * f */
		p = _mm256_add_ps(p, c1);              /* c0 * f + c1 */
		p = _mm256_mul_ps(p, f);               /* (c0 * f + c1) * f */
		p = _mm256_add_ps(p, c2);              /* p = (c0 * f + c1) * f + c2 ~= exp2(f) */
		//p = _mm256_fmadd_ps(c0, f, c1);
		//p = _mm256_fmadd_ps(p, f, c2);
		j = _mm256_slli_epi32(i, 23);          /* i << 23 */
		r = _mm256_castsi256_ps(_mm256_add_epi32(j, _mm256_castps_si256(p))); /* r = p * 2^i*/
		return r;
	}
};