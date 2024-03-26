#include "color.h"
#include <intrin.h>

namespace dsry::color
{
	//https://poynton.ca/PDFs/coloureq.pdf

	constexpr uint32_t Mask_ByteSwarNoLsb_32 = 0xfefefefeu;
	constexpr uint32_t Mask_ByteSwarMsb_32 = 0x80808080u;
	constexpr uint32_t Mask_OddBytes_32 = 0x00ff00ffu;
	constexpr uint64_t Mask_OddBytes_64 = 0x00ff00ff00ff00ffull;
	constexpr uint32_t Mask_8b10b_32 = 0b111111110011111111001111111100;
	constexpr uint32_t Mask_Upper2b10b_32 = 0b110000000011000000001100000000;
	constexpr uint32_t Mask_TwoMsb_32 = 0xc0000000u;

	uint32_t map_255_256(uint32_t x)
	{
		return x + (x >> 7);
	}

	uint32_t mid_rgba8(uint32_t a, uint32_t b)
	{
		return (a | b) - (((a ^ b) & Mask_ByteSwarNoLsb_32) >> 1);
	}

	uint32_t lerp_rgba8_(uint32_t a, uint32_t b, uint32_t t)
	{
		uint32_t rb = (a & Mask_OddBytes_32) * (256 - t);
		rb += (b & Mask_OddBytes_32) * t;

		uint32_t ga = ((a >> 8) & Mask_OddBytes_32) * (256 - t);
		ga += ((b >> 8) & Mask_OddBytes_32) * t;

		return (ga & ~Mask_OddBytes_32) | ((rb >> 8) & Mask_OddBytes_32);
	}

	uint32_t lerp_rgba8_bmi(uint32_t a, uint32_t b, uint32_t t)
	{
		uint64_t x = _pdep_u64(a, Mask_OddBytes_64) * (256 - t);
		x += _pdep_u64(b, Mask_OddBytes_64) * t;
		return (uint32_t)_pext_u64(x, ~Mask_OddBytes_64);
	}

	uint32_t adds_rgba8_(uint32_t a, uint32_t b)
	{
		uint32_t nomsb = (a & ~Mask_ByteSwarMsb_32) + (b & ~Mask_ByteSwarMsb_32);
		uint32_t ovf = (nomsb & a) | (nomsb & b) | (a & b);
		ovf = ((ovf & Mask_ByteSwarMsb_32) >> 7) * 0xffu;
		return ovf | nomsb | ((a | b) & Mask_ByteSwarMsb_32);
	}

	uint32_t adds_rgba8_bmi(uint32_t a, uint32_t b)
	{
		uint64_t x = _pdep_u64(a, Mask_OddBytes_64) + _pdep_u64(b, Mask_OddBytes_64);
		uint64_t xovf = ((x >> 8) & Mask_OddBytes_64) * 0xffu; // 0xff where saturation must occur
		return (uint32_t)_pext_u64(x | xovf, Mask_OddBytes_64);
	}

	uint32_t cvt_rgba8_rgba10_bmi(uint32_t x)
	{
		uint32_t ret = _pdep_u32(x, Mask_8b10b_32) | (x & Mask_TwoMsb_32);
		// replicate RGB upper 2 bits (abcdefgh) -> (abcdefghab)
		return ret | ((ret & Mask_Upper2b10b_32) >> 8);
	}

	uint32_t cvt_rgba10_rgba8_bmi(uint32_t x)
	{
		uint32_t alpha = (x >> 30) * 0x55u; // replicate 2bit alpha into 8bit (ab) -> (abababab)
		return _pext_u32(x, Mask_8b10b_32) | (alpha << 24);
	}

};