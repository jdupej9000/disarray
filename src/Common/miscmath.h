#pragma once
#include <stdint.h>

namespace dsry::math
{
	size_t round_up(size_t x, size_t blk);
	bool is_power_of_two(size_t x);
	uint64_t gcd(uint64_t u, uint64_t v);
};