#include "miscmath.h"
#include <stdint.h>
#include <memory>
#include "config.h"

namespace dsry::math
{
	size_t round_up(size_t x, size_t blk)
	{
		return x + (blk - 1) & (-(ptrdiff_t)blk);
	}

	bool is_power_of_two(size_t x)
	{
		return (x & (x - 1)) == 0;
	}

	uint64_t gcd(uint64_t u, uint64_t v)
	{
		// This is the noswap algorithm from https://lemire.me/blog/2024/04/13/greatest-common-divisor-the-extended-euclidean-algorithm-and-speed/
		if dsry_unlikely(u == 0 || v == 0) 
			return u + v;
	
		auto shift = _tzcnt_u64(u | v);
		u >>= _tzcnt_u64(u);
		do
		{
			uint64_t t = v >> _tzcnt_u64(v);
			if (u > t) v = u - t, u = t;
			else v = t - u;
		} while (v != 0);

		return u << shift;
	}
};
