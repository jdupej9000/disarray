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

int main()
{
	::SetPriorityClass(::GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	::SetThreadAffinityMask(::GetCurrentThread(), 0x4);
	::SwitchToThread();

	while (1)
	{
		benchres br0 = measure<uint32_t>(dsry::color::adds_rgba8_);
		benchres br1 = measure<uint32_t>(dsry::color::adds_rgba8_bmi);

		print_benchres("adds_rgba8_", br0);
		print_benchres("adds_rgba8_bmi", br1);
	}
}


void print_benchres(const char* name, const benchres& br)
{
	cout << name << string(20 - strlen(name), ' ') << endl;
	cout << "   " << br.secsPerRepClean * 1e9 << " ns" << endl;
	cout << "   " << br.ovr_ns << "/" << br.dut_ns << " overhead" << endl;
	cout << endl;
}