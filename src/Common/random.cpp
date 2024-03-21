#include "random.h"
#include "config.h"


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