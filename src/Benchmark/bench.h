#pragma once

#include <chrono>
#include <algorithm>
#include <numeric>

#include <iostream>
#include <fstream>

struct benchtask
{
	void* fnptr;
	int64_t* times;
	int64_t repCount;
	int64_t setCount;
	void* genfnptr;
};

extern "C" void sample_fungen_x64(benchtask* pbt);
extern "C" void sample_fun_x64(benchtask* pbt);
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
	
	void bench_init(void);
	double get_tsc_duration(void) noexcept;
	uint64_t estimate_reps(void* dut, uint64_t nsTarget);
	double ticksToSecs(int64_t t);

	template<typename TRet, typename... TArgs>
	TRet blank(TArgs...)
	{
		return 0;
	}


	template<typename TRet, typename... TArgs>
	//benchresult bench(typename TRet(*dut)(TArgs...), typename void(*gen)(uint64_t, TArgs&...)=nullptr)
	benchresult bench(TRet(*dut)(TArgs...), void* gen = nullptr)
	{
		constexpr int64_t OptimalRuntimeNs = 200000000ull;
		constexpr int64_t OptimalSets = 65536;

		int64_t optimalTotalReps = estimate_reps(dut, OptimalRuntimeNs);
		int64_t numSets = (optimalTotalReps > OptimalSets) ? OptimalSets : optimalTotalReps;
		int64_t numReps = optimalTotalReps / numSets;

		int64_t* timeDut = new int64_t[numSets];
		int64_t* timeCtl = new int64_t[numSets];

		benchtask bt{};
		bt.repCount = numReps;
		bt.setCount = numSets;
		bt.genfnptr = gen;

		bt.times = timeCtl;
		bt.fnptr = blank<TRet, TArgs...>;

		if (gen)
			sample_fungen_x64(&bt);
		else
			sample_fun_x64(&bt);

		bt.times = timeDut;
		bt.fnptr = dut;
		if (gen)
			sample_fungen_x64(&bt);
		else
			sample_fun_x64(&bt);

		std::sort(timeCtl, timeCtl + numSets);
		std::sort(timeDut, timeDut + numSets);

		/*std::ofstream myfile;
		myfile.open("d:\\bench.txt");
		for (uint64_t i = 0; i < numSets; i++)
			myfile << timeCtl[i] << "," << timeDut[i] << std::endl;
		myfile.close();*/


		int64_t i0 = numSets / 4;
		int64_t i1 = numSets * 3 / 4;

		int64_t sumDiff = 0;
		for (int64_t i = 0; i < numSets; i++)
		{
			//timeDut[i] = (timeDut[i] > timeCtl[i]) ? (timeDut[i] - timeCtl[i]) : 0;
		}


		benchresult ret{};
		ret.totalRuns = numReps * numSets;
		ret.timeMean = ticksToSecs(timeDut[numSets/2] - timeCtl[numSets/2]) / numReps;// / (i1 - i0);
		ret.timeSd = abs((ticksToSecs(timeDut[numSets * 3 / 4] - timeCtl[numSets * 3 / 4]) -
			ticksToSecs(timeDut[numSets / 4] - timeCtl[numSets / 4])) / numReps);

		delete[] (timeDut, timeCtl);

		return ret;
	}

	/*benchres2 measure(void* dut, void* ctl = blank<uint32_t>)
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
	}*/
};