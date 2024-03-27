#pragma once

#include <chrono>
#include <stdint.h>

/*#define DSRY_MEASURE_NANO(var,fun) uint64_t var; \
	{ \
		auto ___t0 = std::chrono::high_resolution_clock::now(); \
		auto ___t1 = std::chrono::high_resolution_clock::now(); \
		fun; \
		var = std::chrono::duration_cast<std::chrono::nanoseconds>(___t1 - ___t0).count();	\
	}
*/


namespace dsry::bench
{
	struct bench_result
	{
		uint64_t reps;
		double dutTime;
		uint64_t debug;
	};

	/*class some_bench
	{
	public:
		some_bench(void) {}
		~some_bench(void) {}

		uint64_t overhead(void)
		{
			return 0;
		}

		uint64_t dut(void)
		{
			return 0;
		}
	};*/

#pragma optimize("", off)
	template<typename TBench>
	bench_result measure(void)
	{
		TBench bench{};

		volatile uint64_t noopt = 0;

		auto t0 = std::chrono::high_resolution_clock::now();
		noopt += bench.dut();
		auto t1 = std::chrono::high_resolution_clock::now();

		uint64_t ns_one = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
		if (ns_one == 0) ns_one = 1;

		uint64_t reps = 1000000000ull;// / ns_one;

		auto t2 = std::chrono::high_resolution_clock::now();
		for (uint64_t i = 0; i < reps; i++)
		{
			noopt += bench.overhead();
		}
		auto t3 = std::chrono::high_resolution_clock::now();
		for (uint64_t i = 0; i < reps; i++)
		{
			noopt += bench.dut();
		}
		auto t4 = std::chrono::high_resolution_clock::now();

		int64_t nsdut = std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count();
		int64_t nsovr = std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count();

		bench_result ret{};
		ret.reps = reps;
		ret.dutTime = 1e-9 * (double)std::max(0ll, nsdut - nsovr) / (double)reps;
		ret.debug = noopt;

		return ret;
	}
#pragma optimize("", on)
};