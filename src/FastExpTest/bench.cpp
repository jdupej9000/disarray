#include "bench.h"

#include "exp_internal.h"
#include "fast_exp.h"
#include "fast_exp_fma.h"
#include "very_fast_exp.h"
#include "morpho_exp.h"
#include "schraudolph_exp.h"

#include <immintrin.h>
#include <iostream>
#include <windows.h>

volatile float g_x;

void forget_me_not(__m256 x)
{
	g_x = x.m256_f32[0] - x.m256_f32[5];
	//std::cout << g_x << std::endl;
}

double measure_slow(float* pQ)
{
	float y0 = 0, y1 = 0, y2 = 0, y3 = 0;

	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < 10000000; i++)
	{
		const float* pX = pQ + (i & 0x7) * 8;
		y0 += expf(pX[0]);
		y1 += expf(pX[1]);
		y2 += expf(pX[2]);
		y3 += expf(pX[3]);
		y0 += expf(pX[4]);
		y1 += expf(pX[5]);
		y2 += expf(pX[6]);
		y3 += expf(pX[7]);
	}

	auto end = std::chrono::high_resolution_clock::now();

	printf("%f", y0 * y1 / y2 * y3);

	delete[] pQ;

	return 1e-9 * (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

void execute()
{
	::SetPriorityClass(::GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	::SwitchToThread();

	run_bench<internal_exp>();
	run_bench<morpho_exp>();
	run_bench<fast_exp>();
	run_bench<fast_exp_fma>();
	run_bench<very_fast_exp>();
	run_bench<schraudolph_exp>();
}