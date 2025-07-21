// MemoryBandwidth.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <common.h>
#include <chrono>
#include <thread>

#define NOMINMAX
#include <Windows.h>
#include <immintrin.h>

using namespace std;
using namespace std::chrono;

void measure(size_t n);
float measure_write(uint8_t* p, size_t bytes, size_t passes);
float measure_read(uint8_t* p, size_t bytes, size_t passes);

int main()
{
    cout << "Setting hybrid scheduling policy." << endl;
    dsry::system::set_process_hybrid_policy(dsry::system::HYBRID_POLICY::high_performance);
    
    size_t num_cpus = dsry::system::get_num_cpus();
    cout << "Detected " << num_cpus << " CPUs." << endl;

    cout << "Testing single core bandwidth." << endl;
    measure(num_cpus);
}


void measure(size_t n)
{
    constexpr size_t BlockSize = 20 * 1048576ull;
    constexpr size_t Passes = 100;

    cout << "Writing " << Passes << "x " << BlockSize / 1048576.0f << " MB." << endl;   

    uint8_t* mem = (uint8_t*)VirtualAlloc(NULL, BlockSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    float* bw = new float[2 * n];

    for (int k = 0; k < n; k++) {

        thread t = thread([&] {
            dsry::system::pin_thread_to_cpu(k);
            dsry::system::set_thread_highest_priority();
                     

            bw[2 * k] = measure_write(mem, BlockSize, Passes);
            bw[2 * k + 1] = measure_read(mem, BlockSize, Passes);
        });

        t.join();
        cout << ".";
    }
    
    cout << endl;

    string col_names[2] = { "Write", "Read" };

    dsry::console::table_writer wr{ (int)2, (int)n };
    wr.set_column_width(4);
    wr.set_padding(1, 0);
    wr.set_data_matrix(bw, 2, 4, 1);
    wr.set_column_names(col_names);

    cout << "Single core bandwidth in GB/s:" << endl;
    wr.write(cout);   

    VirtualFree(mem, 0, MEM_RELEASE);
    delete[] bw;
}

float measure_write(uint8_t* p, size_t bytes, size_t passes)
{
    __m256 ones = _mm256_set1_ps(1);

    nanoseconds rtt = nanoseconds::max();

    for (size_t j = 0; j < passes; j++) {
        steady_clock::time_point ts1 = steady_clock::now();

        for (size_t i = 0; i < bytes; i += 128) {
            _mm256_stream_ps((float*)(p + i), ones);
            _mm256_stream_ps((float*)(p + i + 32), ones);
            _mm256_stream_ps((float*)(p + i + 64), ones);
            _mm256_stream_ps((float*)(p + i + 96), ones);
        }

        rtt = std::min(rtt, steady_clock::now() - ts1);
    }

    float seconds = rtt.count() * 1e-9f;
    float gb = bytes / (1048576.0f * 1024.0f);

    return gb / seconds;
}

#pragma optimize("", off)
float measure_read(uint8_t* p, size_t bytes, size_t passes)
{
    nanoseconds rtt = nanoseconds::max();

    for (size_t j = 0; j < passes; j++) {

        __m256i a0 = _mm256_setzero_si256(),
            a1 = _mm256_setzero_si256(),
            a2 = _mm256_setzero_si256(),
            a3 = _mm256_setzero_si256();

        steady_clock::time_point ts1 = steady_clock::now();

        for (size_t i = 0; i < bytes; i += 128) {
            _mm256_stream_load_si256((const __m256i*)(p + i));
            _mm256_stream_load_si256((const __m256i*)(p + i + 32));
            _mm256_stream_load_si256((const __m256i*)(p + i + 64));
            _mm256_stream_load_si256((const __m256i*)(p + i + 96));
        }

        rtt = std::min(rtt, steady_clock::now() - ts1);
    }

    float seconds = rtt.count() * 1e-9f;
    float gb = bytes / (1048576.0f * 1024.0f);

    return gb / seconds;
}