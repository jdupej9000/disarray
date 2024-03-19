#pragma once

#include "bench.h"

struct internal_exp
{
	static std::string get_name()
	{
		return "_mm256_exp_ps";
	}

	static __m256 test(__m256 x) noexcept
	{
		return _mm256_exp_ps(x);
	}
};