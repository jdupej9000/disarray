// TimeTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "utils.h"
#include <iostream>
#include <chrono>
#include <intrin.h>
#include <windows.h>

using namespace std;

double measure_rdtsc(void);
double measure_perfcnt(void);
double measure_GetTickCount(void);
double measure_GetTickCount64(void);
void print_clock_info(const char* name, double period, double query);

int main()
{
    print_clock<chrono::high_resolution_clock>();
    print_clock<chrono::steady_clock>();
    print_clock<chrono::system_clock>();

    print_clock_info("CPU Time Stamp Counter", measure_rdtsc(), measure_query_time<uint64_t>(
        [](uint64_t& li) { li = 0;  },
        [](uint64_t& li) { li = __rdtsc();  }));

    print_clock_info("clock", 1.0 / CLOCKS_PER_SEC, measure_query_time<clock_t>(
        [](clock_t& li) { li = 0;  },
        [](clock_t& li) { li = clock();  }));

    print_clock_info("time", 1.0, measure_query_time<time_t>(
        [](time_t& li) { li = 0;  },
        [](time_t& li) { li = time(nullptr);  }));

    print_clock_info("Windows Perf Counter", measure_perfcnt(), measure_query_time<LARGE_INTEGER>(
        [](LARGE_INTEGER& li) { li.QuadPart = 0;  },
        [](LARGE_INTEGER& li) { ::QueryPerformanceCounter(&li);  }));

    print_clock_info("GetTickCount", measure_GetTickCount(), measure_query_time<DWORD>(
        [](DWORD& li) { li = 0;  },
        [](DWORD& li) { li = ::GetTickCount();  }));

    print_clock_info("GetTickCount64", measure_GetTickCount64(), measure_query_time<ULONGLONG>(
        [](ULONGLONG& li) { li = 0;  },
        [](ULONGLONG& li) { li = ::GetTickCount64();  }));
}

void print_clock_info(const char* name, double period, double query)
{
    std::cout << name << std::endl;
    std::cout << "   tick   : " << make_time(period) << " (" << make_frequency(period) << ")" << std::endl;
    std::cout << "   query  : " << make_time(query) << std::endl;
    std::cout << std::endl;
}

double measure_rdtsc(void)
{
    auto t0 = std::chrono::high_resolution_clock::now();
    uint64_t r0 = __rdtsc();

    Sleep(100);

    auto t1 = std::chrono::high_resolution_clock::now();
    uint64_t r1 = __rdtsc();
    
    return 1e-9 * (double)std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() / (double)(r1 - r0);
}

double measure_GetTickCount(void)
{
    auto t0 = std::chrono::high_resolution_clock::now();
    DWORD r0 = GetTickCount();

    Sleep(1000);

    auto t1 = std::chrono::high_resolution_clock::now();
    DWORD r1 = GetTickCount();

    return 1e-9 * (double)std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() / (double)(r1 - r0);
}

double measure_GetTickCount64(void)
{
    auto t0 = std::chrono::high_resolution_clock::now();
    ULONGLONG r0 = GetTickCount64();

    Sleep(1000);

    auto t1 = std::chrono::high_resolution_clock::now();
    ULONGLONG r1 = GetTickCount64();

    return 1e-9 * (double)std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() / (double)(r1 - r0);
}

double measure_perfcnt(void)
{
    LARGE_INTEGER li;
    ::QueryPerformanceFrequency(&li);
    return 1.0 / li.QuadPart;
}
