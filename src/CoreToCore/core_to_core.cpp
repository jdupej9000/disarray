#include "core_to_core.h"

#include <common.h>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;
using namespace dsry::system;

constexpr int NumSamples = 1000;
constexpr int NumFlips = 100;
constexpr size_t CacheLineSize = 64;

// This is based on: https://github.com/rigtorp/c2clat/blob/master/c2clat.cpp

void measure_c2clat_read(float* plat, size_t n)
{
    const int nsamples = NumSamples;

    for (size_t i = 0; i < n; i++) {
        plat[i * n + i] = 0;

        for (size_t j = i + 1; j < n; ++j) {

            alignas(CacheLineSize) atomic<int> seq1 = { -1 };
            alignas(CacheLineSize) atomic<int> seq2 = { -1 };

            thread t = thread([&] {
                pin_thread_to_cpu(i);
                set_thread_highest_priority();

                for (int m = 0; m < nsamples; m++) {
                   
                    for (int n = 0; n < NumFlips; n++) {
                        while (seq1.load(std::memory_order_acquire) != n) {
                        }

                        seq2.store(n, std::memory_order_release);
                    }                   
                }
                });

            nanoseconds rtt = nanoseconds::max();

            pin_thread_to_cpu(j);

            for (int m = 0; m < nsamples; m++) {
                seq1 = seq2 = -1;               
                steady_clock::time_point ts1 = steady_clock::now();

                for (int n = 0; n < NumFlips; n++) {
                    seq1.store(n, std::memory_order_release);
                    
                    while (seq2.load(std::memory_order_acquire) != n) {                            
                    }
                }

                steady_clock::time_point ts2 = steady_clock::now();    
                rtt = min(rtt, ts2 - ts1);
            }

            t.join();

            float latency_ns = (float)rtt.count() / 2 / NumFlips;
            plat[i * n + j] = latency_ns;
            plat[j * n + i] = latency_ns;
        }

        cout << ".";
    }
}

void measure_c2clat_write(float* plat, size_t n)
{
    const int nsamples = NumSamples;

    for (size_t i = 0; i < n; i++) {
        plat[i * n + i] = 0;

        for (size_t j = i + 1; j < n; ++j) {

            alignas(CacheLineSize) atomic<int> seq1 = { -1 };
            alignas(CacheLineSize) atomic<int> seq2 = { -1 };

            thread t = thread([&] {
                pin_thread_to_cpu(i);
                set_thread_highest_priority();

                for (int m = 0; m < nsamples; m++) {

                    while (seq2.load(std::memory_order_acquire) != 0) {
                    }

                    seq2.store(1, std::memory_order_release);

                    for (int n = 0; n < NumFlips; n++) {
                        int cmp;
                        do {
                            cmp = 2 * n;
                        } while (!seq1.compare_exchange_strong(cmp, cmp + 1));
                    }
                }
                });

            nanoseconds rtt = nanoseconds::max();

            pin_thread_to_cpu(j);

            for (int m = 0; m < nsamples; m++) {
                seq2.store(0, std::memory_order_release);
                while (seq2.load(std::memory_order_acquire) == 0) {
                }

                seq2.store(-1, std::memory_order_release);

                steady_clock::time_point ts1 = steady_clock::now();
                for (int n = 0; n < NumFlips; n++) {
                    int cmp;
                    do {
                        cmp = 2 * n - 1;
                    } while (!seq1.compare_exchange_strong(cmp, cmp + 1));
                }
                
                while (seq1.load(std::memory_order_acquire) != 2 * NumFlips - 1) {
                }

                steady_clock::time_point ts2 = steady_clock::now();
                rtt = min(rtt, ts2 - ts1);
            }

            t.join();            

            float latency_ns = (float)rtt.count() / 2 / NumFlips;
            plat[i * n + j] = latency_ns;
            plat[j * n + i] = latency_ns;
        }

        cout << ".";
    }
}