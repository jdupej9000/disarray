#pragma once

#include <chrono>
#include <algorithm>

struct benchtask
{
	void* fnptr;
	uint64_t* times;
	uint64_t repCount;
	uint64_t setCount;
};

extern "C" uint64_t sample_fun_x64(benchtask* pbt);
extern "C" uint64_t measure_fun2_x64(uint64_t reps, void* funPtr);

namespace dsry::bench
{
	struct benchres
	{
		uint64_t reps, dut_ns, ovr_ns;
		double secsPerRepClean;
		uint64_t dummy;
	};

	

	template<typename TRet>
	TRet blank(uint64_t x)
	{
		return 0;
	}

	template<typename TRet>
	benchres measure(TRet(*dut)(uint32_t, uint32_t), TRet(*ctl)(uint64_t) = blank<TRet>)
	{
		volatile TRet noopt = 0;
		
		uint64_t numRep = 100000000;

		uint64_t totalNanosOverhead = measure_fun2_x64(numRep, (void*)ctl);
		uint64_t totalNanos = measure_fun2_x64(numRep, (void*)dut);

		if (totalNanosOverhead > totalNanos)
			totalNanosOverhead = totalNanos;

		benchres ret{};
		ret.reps = numRep;
		ret.dut_ns = totalNanos;
		ret.ovr_ns = totalNanosOverhead;
		ret.secsPerRepClean = 1e-9 * (double)(totalNanos - totalNanosOverhead) / (double)numRep;
		ret.dummy = noopt;

		return ret;
	};

	void measure(void* dut, void* ctl = blank<uint32_t>)
	{
		constexpr uint64_t NumReps = 1048576;
		constexpr uint64_t NumSets = 92;
		uint64_t* tDut = new uint64_t[NumSets];
		uint64_t* tCtl = new uint64_t[NumSets];

		benchtask bt{};
		bt.repCount = NumReps;
		bt.setCount = NumSets;
		
		bt.times = tCtl;
		bt.fnptr = ctl;
		sample_fun_x64(&bt);

		bt.times = tDut;
		bt.fnptr = dut;
		sample_fun_x64(&bt);

		std::sort(tCtl, tCtl + NumSets);
		std::sort(tDut, tDut + NumSets);



		delete[](tDut, tCtl);
	}
};