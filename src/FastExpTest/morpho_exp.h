#pragma once

#include "bench.h"

#define BROADCAST8(x)  {(x),(x),(x),(x), (x),(x),(x),(x)}
#define BROADCAST16(x)  {(x),(x),(x),(x), (x),(x),(x),(x), (x),(x),(x),(x), (x),(x),(x),(x)}
#define GVAR256F(name, x) __declspec(align(16)) float g_##name[8] = BROADCAST8(x)
#define GVAR256FY(name, x) __declspec(align(16)) const __m256 g_y_##name = BROADCAST8(x)
#define GVAR512FY(name, x) __declspec(align(64)) const __m512 g_z_##name = BROADCAST16(x)
#define GVARFY(name, x) GVAR256FY(name, x); GVAR512FY(name, x)
#define GVAR256IY(name, x) __declspec(align(16)) const __m256i g_##name = BROADCAST8(x)

GVARFY(exp_hi, 88.3762626647949f);
GVARFY(exp_lo, -88.3762626647949f);

GVARFY(One, 1.0f);
GVARFY(Half, 0.5f);
//GVAR256IY(ExpMask, 0x7f);
GVARFY(cephes_LOG2EF, 1.44269504088896341);
GVARFY(cephes_exp_C1, 0.693359375);
GVARFY(cephes_exp_C2, -2.12194440e-4);

GVARFY(cephes_exp_p0, 1.9875691500E-4);
GVARFY(cephes_exp_p1, 1.3981999507E-3);
GVARFY(cephes_exp_p2, 8.3334519073E-3);
GVARFY(cephes_exp_p3, 4.1665795894E-2);
GVARFY(cephes_exp_p4, 1.6666665459E-1);
GVARFY(cephes_exp_p5, 5.0000001201E-1);

constexpr float g_cephes_exp_p[6] = { 1.9875691500E-4f, 1.3981999507E-3f, 8.3334519073E-3f, 4.1665795894E-2f, 1.6666665459E-1f, 5.0000001201E-1f };
constexpr float g_cephes_exp_c = 0.693359375f - 2.12194440e-4f;


struct morpho_exp
{
	static std::string get_name()
	{
		return "morpho_exp";
	}

	static __m256 test(__m256 x) noexcept
	{
		// has 1 where the floats hold their exponents, 0 otherwise
		__m256i expMask = _mm256_set1_epi32(0x0000007f);

		__m256 tmp, fx;
		__m256i imm0;

		x = _mm256_min_ps(x, g_y_exp_hi);
		x = _mm256_max_ps(x, g_y_exp_lo);

		/* express exp(x) as exp(g + n*log(2)) */
		fx = _mm256_fmadd_ps(x, g_y_cephes_LOG2EF, g_y_Half);
		tmp = _mm256_floor_ps(fx);

		/* fx[i] = (tmp[i] > fx[i]) ? tmp[i] - 1 : tmp[i] */
		__m256 one = g_y_One;
		__m256 mask = _mm256_cmp_ps(tmp, fx, _CMP_GT_OS);
		mask = _mm256_and_ps(mask, one);
		fx = _mm256_sub_ps(tmp, mask);

		tmp = _mm256_mul_ps(fx, g_y_cephes_exp_C1);
		__m256 z = _mm256_mul_ps(fx, g_y_cephes_exp_C2);
		x = _mm256_sub_ps(x, tmp);
		x = _mm256_sub_ps(x, z);

		z = _mm256_mul_ps(x, x);

		__m256 y = g_y_cephes_exp_p0;
		y = _mm256_fmadd_ps(y, x, g_y_cephes_exp_p1);
		y = _mm256_fmadd_ps(y, x, g_y_cephes_exp_p2);
		y = _mm256_fmadd_ps(y, x, g_y_cephes_exp_p3);
		y = _mm256_fmadd_ps(y, x, g_y_cephes_exp_p4);
		y = _mm256_fmadd_ps(y, x, g_y_cephes_exp_p5);
		y = _mm256_fmadd_ps(y, z, x);
		y = _mm256_add_ps(y, one);

		/* build 2^n */
		imm0 = _mm256_cvttps_epi32(fx);
		imm0 = _mm256_add_epi32(imm0, expMask);
		imm0 = _mm256_slli_epi32(imm0, 23);
		__m256 pow2n = _mm256_castsi256_ps(imm0);
		y = _mm256_mul_ps(y, pow2n);
		return y;
	}
};