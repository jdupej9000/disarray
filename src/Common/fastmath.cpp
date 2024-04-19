#include "fastmath.h"

namespace dsry::math
{
	__m256 expf_fast(__m256 x) noexcept
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
		p = _mm256_fmadd_ps(c0, f, c1);			/* c0 * f + c1 */
		p = _mm256_fmadd_ps(p, f, c2);			/* p = (c0 * f + c1) * f + c2 ~= exp2(f) */
		j = _mm256_slli_epi32(i, 23);          /* i << 23 */
		r = _mm256_castsi256_ps(_mm256_add_epi32(j, _mm256_castps_si256(p))); /* r = p * 2^i*/
		return r;
	}

	__m512 expf_fast(__m512 x) noexcept
	{
		__m512 t, f, p, r;
		__m512i i, j;

		const __m512 l2e = _mm512_set1_ps(1.442695041f); /* log2(e) */
		const __m512 cvt = _mm512_set1_ps(12582912.0f);  /* 1.5 * (1 << 23) */
		const __m512 c0 = _mm512_set1_ps(0.238428936f);
		const __m512 c1 = _mm512_set1_ps(0.703448006f);
		const __m512 c2 = _mm512_set1_ps(1.000443142f);

		/* exp(x) = 2^i * 2^f; i = rint (log2(e) * x), -0.5 <= f <= 0.5 */
		t = _mm512_mul_ps(x, l2e);             /* t = log2(e) * x */
		r = _mm512_sub_ps(_mm512_add_ps(t, cvt), cvt); /* r = rint (t) */
		f = _mm512_sub_ps(t, r);               /* f = t - rint (t) */
		i = _mm512_cvtps_epi32(t);             /* i = (int)t */
		p = _mm512_fmadd_ps(c0, f, c1);			/* c0 * f + c1 */
		p = _mm512_fmadd_ps(p, f, c2);			/* p = (c0 * f + c1) * f + c2 ~= exp2(f) */
		j = _mm512_slli_epi32(i, 23);          /* i << 23 */
		r = _mm512_castsi512_ps(_mm512_add_epi32(j, _mm512_castps_si512(p))); /* r = p * 2^i*/
		return r;
	}

	__m256 expf_schraudolph(__m256 x) noexcept
	{
		__m256 a = _mm256_set1_ps(12102203.0f); /* (1 << 23) / log(2) */
		__m256i b = _mm256_set1_epi32(127 * (1 << 23) - 298765);
		__m256i t = _mm256_add_epi32(_mm256_cvtps_epi32(_mm256_mul_ps(a, x)), b);
		return _mm256_castsi256_ps(t);
	}

	__m512 expf_schraudolph(__m512 x) noexcept
	{
		__m512 a = _mm512_set1_ps(12102203.0f); /* (1 << 23) / log(2) */
		__m512i b = _mm512_set1_epi32(127 * (1 << 23) - 298765);
		__m512i t = _mm512_add_epi32(_mm512_cvtps_epi32(_mm512_mul_ps(a, x)), b);
		return _mm512_castsi512_ps(t);
	}

	float reduce_add(__m256 x) noexcept
	{
		const __m128 x128 = _mm_add_ps(_mm256_extractf128_ps(x, 1), _mm256_castps256_ps128(x));
		const __m128 x64 = _mm_add_ps(x128, _mm_movehl_ps(x128, x128));
		const __m128 x32 = _mm_add_ss(x64, _mm_shuffle_ps(x64, x64, 0x55));
		return _mm_cvtss_f32(x32);
	}
};