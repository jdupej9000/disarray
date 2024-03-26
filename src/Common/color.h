#pragma once

#include <stdint.h>

// Maps the lower scalar's interval [0x0,0xff] to [0x0, 0x100].
uint32_t map_255_256(uint32_t x);

// Find (a+b)/2 for packed RGBA8 values.
uint32_t mid_rgba8(uint32_t a, uint32_t b);

// Linearly interpolate between a and b at the position t, where
// t is in [0, 0x100].
uint32_t lerp_rgba8(uint32_t a, uint32_t b, uint32_t t);
uint32_t lerp_rgba8_bmi(uint32_t a, uint32_t b, uint32_t t);

// Add packed RGBA8 values a+b and saturate each channel.
uint32_t adds_rgba8(uint32_t a, uint32_t b);
