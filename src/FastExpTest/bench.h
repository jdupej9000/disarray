#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <immintrin.h>

// https://stackoverflow.com/questions/47025373/fastest-implementation-of-the-natural-exponential-function-using-sse

constexpr size_t BenchReps = 10000000;
constexpr size_t RepSize = 8 * 8;

struct control
{
	static std::string get_name()
	{
		return "control";
	}

	__declspec(noinline) static __m256 test(__m256 x) noexcept
	{
		return _mm256_setzero_ps();
	}
};



void forget_me_not(__m256 x);
void execute();

template<typename TBench>
double measure()
{
	float* pQ = new float[] {
		1.0f, 2.0f, 3.0f, 4.0f, 10.0f, 20.0f, 30.0f, 40.0f, 
		1.0f, 2.0f, 3.0f, 4.0f, 10.0f, 20.0f, 30.0f, 40.0f, 
		1.0f, 2.0f, 3.0f, 4.0f, 10.0f, 20.0f, 30.0f, 40.0f, 
		1.0f, 2.0f, 3.0f, 4.0f, 10.0f, 20.0f, 30.0f, 40.0f, 
		1.0f, 2.0f, 3.0f, 4.0f, 10.0f, 20.0f, 30.0f, 40.0f, 
		1.0f, 2.0f, 3.0f, 4.0f, 10.0f, 20.0f, 30.0f, 40.0f, 
		1.0f, 2.0f, 3.0f, 4.0f, 10.0f, 20.0f, 30.0f, 40.0f, 
		1.0f, 2.0f, 3.0f, 4.0f, 10.0f, 20.0f, 30.0f, 40.0f};

	__m256 y = _mm256_setzero_ps();

	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < BenchReps; i++)
	{
		__m256 a = TBench::test(_mm256_load_ps(pQ + (i & 0x7) * 8));
		y = _mm256_xor_ps(y, a);
	}

	auto end = std::chrono::high_resolution_clock::now();

	forget_me_not(y);

	delete[] pQ;

	return 1e-9 * (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

template<typename TBench>
double precision(float x0, float x1)
{
	constexpr size_t n = 10000000;
	double dx = (x1 - x0) / n;

	double max_rel = 0;
	for (size_t i = 0; i < n; i++)
	{
		double x = x0 + i * dx;
		double expx = exp(x);
		float xx = x;

		float expxx = TBench::test(_mm256_set1_ps(xx)).m256_f32[0];

		double rel = abs((double)expxx - expx) / abs(expx);
		if (rel > max_rel) max_rel = rel;
	}

	return max_rel;
}

template<typename TBench>
void run_bench()
{
	double time_dut = 0, time_ctl = 0;
	constexpr int n = 100;

	for (int i = 0; i < n; i++)
	{
		double tctl0 = measure<control>();
		double tdut = measure<TBench>();
		double tctl1 = measure<control>();

		time_ctl += 0.5 * (tctl0 + tctl1);
		time_dut += tdut;
	}
	double t = time_dut - time_ctl;
	double speed = (double)(RepSize * BenchReps) / t;

	double err_1 = precision<TBench>(-1, 1);
	double err_full = precision<TBench>(-87.3365f, 88.7228f);

	std::cout << TBench::get_name() << std::endl;
	std::cout << std::fixed;
	std::cout.precision(2);
	std::cout << "   time        : " << t * 1000.0 << " ms" << std::endl;
	std::cout << "   speed       : " << speed / 1e6 << " Mops/sec" << std::endl;

	std::cout.precision(7);
	std::cout << "   err(-1,1)   : " << err_1 << std::endl;
	std::cout << "   err(-87,87) : " << err_full << std::endl;
	std::cout << std::endl;
}