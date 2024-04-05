// Benchmark.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma comment(lib, "Common.lib")

#include <Windows.h>
#include <iostream>
#include "bench.h"
#include "common.h"

using namespace std;
using namespace dsry::bench;

static const char* pszTPlanaHath = "Logic is the cement of our civilization with which we ascend from chaos, using reason as our guide.";

void print_benchres(const char* name, const benchres& br);
void print_benchres2(const char* name, const benchres2& br);
void print_br(const char* name, const benchresult& br);

void gen_strlen(uint64_t i, const char*& x)
{
	x = pszTPlanaHath;
}

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

	bench_init();
	cout << "TSC duration: " << get_tsc_duration() * 1.0e9 << "ns (" << 1.0e-9 / get_tsc_duration() << "GHz)" << endl;

	//bench(dsry::color::lerp_rgba8_bmi);

	//return 0;

	cout << endl;

	uint8_t dest[64];


	while (1)
	{
		benchresult br = bench(
			blank<uint32_t, uint32_t, uint32_t>);
			//gen_simple_eeb);
		print_br("blank", br);

		//br = bench(strlen, gen_strlen);
		//gen_simple_ee);
		//print_br("strlen", br);

		br = bench(
			dsry::color::adds_rgba8_);
			//gen_simple_ee);
		print_br("adds_rgba8_", br);

		br = bench(
			dsry::color::adds_rgba8_bmi);
			//gen_simple_ee);
		print_br("adds_rgba8_bmi", br);

		br = bench(
			dsry::color::mid_rgba8);
			//gen_simple_ee);
		print_br("mid_rgba8", br);
	
		br = bench(
			dsry::color::lerp_rgba8_);
			//gen_simple_eeb);
		print_br("lerp_rgba8_", br);
		
		br = bench(
			dsry::color::lerp_rgba8_bmi);
			//gen_simple_eeb);
		print_br("lerp_rgba8_bmi", br);

		br = bench(dsry::color::cvt_rgba8_rgba10_bmi);
		print_br("cvt_rgba8_rgba10_bmi", br);

		br = bench(dsry::color::cvt_rgba10_rgba8_bmi);
		print_br("cvt_rgba10_rgba8_bmi", br);
	
		cout << endl;
	}
}

void print_br(const char* name, const benchresult& br)
{
	cout.precision(2);
	cout << fixed;
	cout << name << string(24 - strlen(name), ' ') <<
		br.timeMean * 1e9 << "ns " <<
		"iqr=" << br.timeSd * 1e9 << "ns " <<
		"runs=" << br.totalRuns << endl;
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