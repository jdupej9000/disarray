#pragma once
#include <intrin.h>

namespace dsry::math
{
	__m256 expf_fast(__m256 x) noexcept;
	__m512 expf_fast(__m512 x) noexcept;
	__m256 expf_schraudolph(__m256 x) noexcept;
	__m512 expf_schraudolph(__m512 x) noexcept;

	float reduce_add(__m256 x) noexcept;
};