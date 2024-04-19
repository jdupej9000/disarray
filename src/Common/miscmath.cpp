#include "miscmath.h"
#include <stdint.h>
#include <memory>

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
};
