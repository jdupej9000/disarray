#include "common.h"
#include "bench.h"
#include <intrin.h>
#include <iostream>

using namespace std;
using namespace dsry::color;

namespace dsry::bench
{
	class bench_adds_
	{
	public:
		bench_adds_(void) 
		{
			_a = std::rand();
			_b = std::rand();
		}

		~bench_adds_(void) {}

		uint32_t _a, _b;

		__declspec(noinline) uint64_t overhead(void)
		{
			return _a;
		}

		__declspec(noinline) uint64_t dut(void)
		{
			return adds_rgba8_(_a, _b);
		}
	};

	class bench_adds_bmi
	{
	public:
		bench_adds_bmi(void)
		{
			_a = std::rand();
			_b = std::rand();
		}

		~bench_adds_bmi(void) {}

		uint32_t _a, _b;

		__declspec(noinline) uint64_t overhead(void)
		{
			return _a;
		}

		__declspec(noinline) uint64_t dut(void)
		{
			return adds_rgba8_bmi(_a, _b);
		}
	};

	void benchmark_color(void)
	{
		bench_result plain = measure<bench_adds_>();
		bench_result bmi = measure<bench_adds_bmi>();

		cout << "Plain  : " << plain.dutTime * 1e9 << " ns (" << plain.reps << ")" << endl;
		cout << "BMI    : " << bmi.dutTime * 1e9 << " ns (" << bmi.reps << ")" << endl;
	}
};