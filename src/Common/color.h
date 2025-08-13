#pragma once

#include "config.h"
#include <stdint.h>

#define COLOR_TO_RGB(x) (((x) >> 16) & 0xff), (((x) >> 8) & 0xff), ((x) & 0xff)

namespace dsry::color
{
	// Maps the lower scalar's interval [0x0,0xff] to [0x0, 0x100].
	uint32_t map_255_256(uint32_t x);

	// Find (a+b)/2 for packed RGBA8 values.
	uint32_t mid_rgba8(uint32_t a, uint32_t b);

	// Linearly interpolate between a and b at the position t, where
	// t is in [0, 0x100].
	uint32_t lerp_rgba8_(uint32_t a, uint32_t b, uint32_t t);

	// Add packed RGBA8 values a+b and saturate each channel.
	uint32_t adds_rgba8_(uint32_t a, uint32_t b);

	constexpr uint32_t make_rgba(int r, int g, int b, int a = 0xff) {
		return (a << 24) | (r << 16) | (g << 8) | b;
	}

#if defined(DSRI_BMI)
	uint32_t lerp_rgba8_bmi(uint32_t a, uint32_t b, uint32_t t);
	uint32_t adds_rgba8_bmi(uint32_t a, uint32_t b);

	// Converts between r8g8b8a8 and r10g10b10a2 formats. The latter designator
	// denotes the destination format.
	uint32_t cvt_rgba8_rgba10_bmi(uint32_t x);
	uint32_t cvt_rgba10_rgba8_bmi(uint32_t x);
#endif

};

#if defined(DSRY_BMI)

#define lerp_rgba8 lerp_rgba8_bmi
#define adds_rgba8 adds_rgba8_bmi
#define cvt_rgba8_rgba10 cvt_rgba8_rgba10_bmi
#define cvt_rgba10_rgba8 cvt_rgba10_rgba8_bmi

#else

#pragma message ("color.h uses BMI despite DSRY_BMI being not defined.")
#define lerp_rgba8 lerp_rgba8_
#define adds_rgba8 adds_rgba8_
#define cvt_rgba8_rgba10 (void)
#define cvt_rgba10_rgba8 (void)

#endif
