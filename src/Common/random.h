#pragma once

#include <stdint.h>

uint64_t rand_murmur(uint64_t seed);
uint64_t rand_splitmix(uint64_t& seed);