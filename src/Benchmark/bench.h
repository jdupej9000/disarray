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
	void* genfnptr;
};

extern "C" uint64_t sample_fun_x64(benchtask* pbt);
extern "C" uint64_t measure_fun_x64(void* fun, uint64_t reps);

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

	struct benchresult
	{
		uint64_t totalRuns;
		double timeMean, timeSd;
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

	uint64_t estimate_reps(void* dut, uint64_t nsTarget)
	{
		constexpr uint64_t MaxCount = 1 << 30;
		uint64_t count = 1;

		while (count < MaxCount)
		{
			if (measure_fun_x64(dut, count) >= nsTarget)
				return count;

			count <<= 1;
		}

		return count;
	}

	template<typename TRet, typename ...TArgs>
	benchresult bench(TRet(*dut)(TArgs...), void(*gen)(uint64_t, TArgs&...)=nullptr)
	{
		constexpr uint64_t OptimalRuntimeNs = 1000000000ull;
		constexpr uint64_t OptimalSets = 1024;

		uint64_t optimalTotalReps = estimate_reps(dut, OptimalRuntimeNs);
		uint64_t numSets = (optimalTotalReps > OptimalSets) ? OptimalSets : optimalTotalReps;
		uint64_t numReps = optimalTotalReps / numSets;

		uint64_t* timeDut = new uint64_t[numSets];
		uint64_t* timeCtl = new uint64_t[numSets];

		benchtask bt{};
		bt.repCount = numReps;
		bt.setCount = numSets;
		bt.genfnptr = gen;

		bt.times = timeCtl;
		bt.fnptr = blank<uint32_t>;
		sample_fun_x64(&bt);

		bt.times = timeDut;
		bt.fnptr = dut;
		sample_fun_x64(&bt);

		std::sort(timeCtl, timeCtl + numSets);
		std::sort(timeDut, timeDut + numSets);

		uint64_t i0 = numSets / 5;
		uint64_t i1 = numSets * 4 / 5;
		uint64_t sumDut = 0, sumCtl = 0;
		for (uint64_t i = i0; i < i1; i++)
		{
			sumDut += timeDut[i];
			sumCtl += timeCtl[i];
		}

		benchresult ret{};
		ret.totalRuns = numReps * numSets;
		ret.timeMean = ticksToSecs(sumDut - sumCtl) / numReps / (i1 - i0);

		delete[] (timeDut, timeCtl);

		return ret;
	}

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