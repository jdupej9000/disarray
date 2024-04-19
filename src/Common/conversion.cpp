#include "conversion.h"
#include <intrin.h>

namespace dsry::conversion
{
	void f64_to_f32(float* dest, const double* src, size_t length)
	{
		size_t numBlocks = length >> 4;

		for (size_t i = 0; i < numBlocks; i++)
		{
			const double* srci = src + 16 * i;

			__m128 f0 = _mm256_cvtpd_ps(_mm256_load_pd(srci));
			__m128 f1 = _mm256_cvtpd_ps(_mm256_load_pd(srci + 4));
			__m128 f2 = _mm256_cvtpd_ps(_mm256_load_pd(srci + 8));
			__m128 f3 = _mm256_cvtpd_ps(_mm256_load_pd(srci + 12));

			float* desti = dest + 16 * i;
			__m256 f01 = _mm256_permute2f128_ps(_mm256_castps128_ps256(f0), _mm256_castps128_ps256(f1), 0x20);
			__m256 f23 = _mm256_permute2f128_ps(_mm256_castps128_ps256(f2), _mm256_castps128_ps256(f3), 0x20);

			_mm256_store_ps(desti, f01);
			_mm256_store_ps(desti + 8, f23);
		}

		for (size_t i = numBlocks << 4; i < length; i++)
			dest[i] = (float)src[i];
	}

	void f32_to_f64(double* dest, const float* src, size_t length)
	{
		size_t numBlocks = length >> 4;

		for (size_t i = 0; i < numBlocks; i++)
		{
			const float* srci = src + 16 * i;

			__m256 f0 = _mm256_load_ps(srci);
			__m256 f1 = _mm256_load_ps(srci + 8);

			__m256d d0 = _mm256_cvtps_pd(_mm256_extractf128_ps(f0, 0));
			__m256d d1 = _mm256_cvtps_pd(_mm256_extractf128_ps(f0, 1));
			__m256d d2 = _mm256_cvtps_pd(_mm256_extractf128_ps(f1, 0));
			__m256d d3 = _mm256_cvtps_pd(_mm256_extractf128_ps(f1, 1));

			double* desti = dest + 16 * i;
			_mm256_store_pd(desti, d0);
			_mm256_store_pd(desti + 4, d1);
			_mm256_store_pd(desti + 8, d2);
			_mm256_store_pd(desti + 12, d3);
		}

		for (size_t i = numBlocks << 4; i < length; i++)
			dest[i] = (double)src[i];
	}
};