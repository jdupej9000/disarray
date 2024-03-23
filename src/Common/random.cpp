#include "random.h"
#include "config.h"
#include <intrin.h>


uint64_t rand_murmur(uint64_t seed)
{
    //https://lemire.me/blog/2023/10/17/randomness-in-programming-with-go-code/
    uint64_t h = seed;
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccd;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53;
    h ^= h >> 33;
    return h;
}

uint64_t rand_splitmix(uint64_t& seed)
{
    //https://lemire.me/blog/2023/10/17/randomness-in-programming-with-go-code/
    seed += 0x9E3779B97F4A7C15;
    uint64_t z = seed;
    z = (z ^ (z >> 30));
    z *= (0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27));
    z *= (0x94D049BB133111EB);
    return z ^ (z >> 31);
}

uint64_t rand_wyhash(uint64_t& state)
{
    //https://lemire.me/blog/2021/03/17/apples-m1-processor-and-the-full-128-bit-integer-product/
    state += 0x60bee2bee120fc15ull;
    uint64_t hi, lo;
    lo = _mul128(state, 0xa3b195354a39b70dull, (long long*)&hi);
    uint64_t m1 = hi ^ lo;
    lo = _mul128(m1, 0x1b03738712fad5c9ull, (long long*)&hi);
    return hi ^ lo;
}