// Benchmark.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma comment(lib, "Common.lib")

#include <Windows.h>
#include <iostream>
#include "bench.h"
#include "common.h"

using namespace std;
using namespace dsry::bench;

void print_benchres(const char* name, const benchres& br);
void print_benchres2(const char* name, const benchres2& br);

void gen_simple_ee(uint64_t i, uint32_t& a, uint32_t& b)
{
	a = b = (uint32_t)i;
}

void gen_simple_eeb(uint64_t i, uint32_t& a, uint32_t& b, uint32_t& c)
{
	a = b = (uint32_t)i;
	c = i & 0xff;
}

int main()
{
	::SetPriorityClass(::GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	//::SetThreadAffinityMask(::GetCurrentThread(), 0x4);
	::SwitchToThread();

	measure((void*)&dsry::color::adds_rgba8_);

	cout << "adds_rgba8_, adds_rgba8_bmi, mid_rgba8" << endl;

	while (1)
	{
		benchresult br0 = bench<uint32_t, uint32_t, uint32_t>(
			dsry::color::adds_rgba8_, 
			gen_simple_ee);
		
		cout.precision(2);
		cout << fixed;
		cout << "adds_rgba8_   : " << br0.timeMean * 1e9 << "ns, " << br0.totalRuns << " runs" << endl;

		benchresult br1 = bench<uint32_t, uint32_t, uint32_t>(
			dsry::color::adds_rgba8_bmi,
			gen_simple_ee);

		cout << "adds_rgba8_bmi: " << br1.timeMean * 1e9 << "ns, " << br1.totalRuns << " runs" << endl;

		benchresult br2 = bench<uint32_t, uint32_t, uint32_t>(
			dsry::color::mid_rgba8,
			gen_simple_ee);

		cout << "mid_rgba8     : " << br2.timeMean * 1e9 << "ns, " << br2.totalRuns << " runs" << endl;

		benchresult br3 = bench<uint32_t, uint32_t, uint32_t, uint32_t>(
			dsry::color::lerp_rgba8_,
			gen_simple_eeb);

		cout << "lerp_rgba8_   : " << br3.timeMean * 1e9 << "ns, " << br3.totalRuns << " runs" << endl;

		benchresult br4 = bench<uint32_t, uint32_t, uint32_t, uint32_t>(
			dsry::color::lerp_rgba8_bmi,
			gen_simple_eeb);

		cout << "lerp_rgba8_bmi: " << br4.timeMean * 1e9 << "ns, " << br4.totalRuns << " runs" << endl;

		cout << endl;
	}
}


void print_benchres(const char* name, const benchres& br)
{
	cout << name << string(20 - strlen(name), ' ') << endl;
	cout << "   " << br.secsPerRepClean * 1e9 << " ns" << endl;
	cout << "   " << br.ovr_ns << "/" << br.dut_ns << " overhead" << endl;
	cout << endl;
}

void print_benchres2(const char* name, const benchres2& br)
{
	cout << name << string(20 - strlen(name), ' ') << endl;
	cout << "   " << br.timeAvg * 1e9 << "ns" << endl;
	cout << endl;
}