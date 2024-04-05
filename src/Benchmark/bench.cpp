#include "bench.h"
#include <chrono>
#include <intrin.h>
#include <windows.h>

using namespace std::chrono;

namespace dsry::bench
{
	double g_secondsPerTsc;

	void bench_init(void)
	{
		high_resolution_clock::time_point t0 = high_resolution_clock::now();
		uint64_t tsc0 = __rdtsc();

		::Sleep(172);

		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		uint64_t tsc1 = __rdtsc();

		g_secondsPerTsc = 1.0e-9 * (double)duration_cast<nanoseconds>(t1 - t0).count() / (double)(tsc1-tsc0);
	}

	double get_tsc_duration(void) noexcept
	{
		return g_secondsPerTsc;
	}

	uint64_t estimate_reps(void* dut, uint64_t nsTarget)
	{
		constexpr uint64_t MaxCount = 1 << 27;
		uint64_t count = 1;

		while (count < MaxCount)
		{
			if (measure_fun_x64(dut, count) >= nsTarget)
				return count;

			count <<= 1;
		}

		return count;
	}

	double ticksToSecs(int64_t t)
	{
		if (t < 0) return 0.0;
		return t * g_secondsPerTsc;
	}
};