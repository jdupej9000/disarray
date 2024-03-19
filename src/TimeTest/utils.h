#pragma once

#include <iostream>
#include <chrono>
#include <sstream>
#include <typeinfo>

std::string make_frequency(double period)
{
	std::stringstream ss;
	ss << std::fixed;
	ss.precision(2);

	double f = 1.0 / period;

	if (f >= 0.9e9)
		ss << f * 1e-9 << "GHz";
	else if (f >= 0.9e6)
		ss << f * 1e-6 << "MHz";
	else if (f >= 0.9e-3)
		ss << f * 1e-3 << "kHz";
	else
		ss << f << "Hz";

	return ss.str();
}

std::string make_time(double period)
{
	std::stringstream ss;
	ss << std::fixed;
	ss.precision(2);

	if (period < 0.9e-9)
		ss << period * 1e12 << "ps";
	else if (period < 0.9e-6)
		ss << period * 1e9 << "ns";
	else if (period < 0.9e-3)
		ss << period * 1e6 << "us";
	else if (period < 0.9)
		ss << period * 1e3 << "ms";
	else
		ss << period << "s";

	return ss.str();
}

template<typename TClock>
double measure_query_time(void)
{
	constexpr int N = 10000;
	typename TClock::time_point* xx = new typename TClock::time_point[N];
	
	auto t0 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		xx[i] = TClock::time_point::max();
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		xx[i] = TClock::now();
	}
	auto t2 = std::chrono::high_resolution_clock::now();

	delete[] xx;

	return (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() - std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()) * 1e-9 / N;
}

template<typename TItem>
double measure_query_time(void (*fun0)(TItem&), void (*fun1)(TItem&))
{
	constexpr int N = 10000;
	TItem* xx = new TItem[N];

	auto t0 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		fun0(xx[i]);
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++)
	{
		fun1(xx[i]);
	}
	auto t2 = std::chrono::high_resolution_clock::now();

	delete[] xx;

	return (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() - std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count()) * 1e-9 / N;
}

template<typename TClock>
void print_clock(void)
{
	double period = (double)TClock::period::num / TClock::period::den;

	std::cout << typeid(TClock).name() << std::endl;
	std::cout << "   tick   : " << make_time(period) << " (" << make_frequency(period) << ")" << std::endl;
	std::cout << "   query  : " << make_time(measure_query_time<TClock>()) << std::endl;
	std::cout << "   steady : " << TClock::is_steady << std::endl;
	std::cout << std::endl;
}