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
		//benchres br0 = measure<uint32_t>(dsry::color::adds_rgba8_);
		//benchres br1 = measure<uint32_t>(dsry::color::adds_rgba8_bmi);

		//print_benchres("adds_rgba8_", br0);
		//print_benchres("adds_rgba8_bmi", br1);

		benchres2 br0 = measure((void*)&dsry::color::adds_rgba8_);
		benchres2 br1 = measure((void*)&dsry::color::adds_rgba8_bmi);
		benchres2 br2 = measure((void*)&dsry::color::mid_rgba8);

		cout << br0.timeAvg * 1e9 << ", " << br1.timeAvg * 1e9 << ", " << br2.timeAvg * 1e9 << endl;
		
		//print_benchres2("adds_rgba8_", br0);
		//print_benchres2("adds_rgba8_bmi", br1);
		//print_benchres2("mid_rgba8", br2);
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