#pragma once

#include <chrono>

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
};