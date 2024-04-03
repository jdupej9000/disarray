#pragma once

#include <chrono>
#include <algorithm>
#include <numeric>

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
	

	struct benchres2
	{
		double timeMin, timeMax, time10, time90, timeMedian, timeAvg;
	};
	
	double ticksToSecs(int64_t t)
	{
		if (t < 0) return 0.0;
		// TODO
		return t * 1.0e-9;
	}

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

	benchres2 measure(void* dut, void* ctl = blank<uint32_t>)
	{
		constexpr uint64_t NumReps = 100000; // 1M
		constexpr uint64_t NumSets = 1000;
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

		benchres2 ret{};
		
		ret.timeMin = ticksToSecs(tDut[0] - tCtl[0]) / NumReps;
		ret.timeMax = ticksToSecs(tDut[NumSets - 1] - tCtl[NumSets - 1]) / NumReps;
		ret.time10 = ticksToSecs(tDut[NumSets / 10] - tCtl[NumSets / 10]) / NumReps;
		ret.time90 = ticksToSecs(tDut[NumSets * 9 / 10] - tCtl[NumSets * 9 / 10]) / NumReps;
		ret.timeMedian = ticksToSecs(tDut[NumSets / 2] - tCtl[NumSets / 2]) / NumReps;
		
		uint64_t i0 = NumSets / 5;
		uint64_t i1 = NumSets * 4 / 5;
		uint64_t sumDut = 0, sumCtl = 0;
		for (uint64_t i = i0; i < i1; i++)
		{
			sumDut += tDut[i];
			sumCtl += tCtl[i];
		}

		ret.timeAvg = ticksToSecs(sumDut - sumCtl) / NumReps / (i1-i0);

		delete[](tDut, tCtl);

		return ret;
	}
};