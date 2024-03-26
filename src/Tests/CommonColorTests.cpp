#include "pch.h"
#include "CppunitTest.h"

#include <limits.h>

#define __rgba10(a,r,g,b) (uint32_t)(((a) << 30) | ((r) << 20) | ((g) << 10) | (b))

using namespace dsry::color;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(CommonColor)
	{
	public:
		TEST_METHOD(Map255256)
		{
			Assert::AreEqual(0x0u, map_255_256(0x0u));
			Assert::AreEqual(0x100u, map_255_256(0xffu));
		}

		TEST_METHOD(AddsRgba8)
		{
			Assert::AreEqual(0x00000000u, adds_rgba8_(0x00000000u, 0x00000000u));
			Assert::AreEqual(0x01000001u, adds_rgba8_(0x01000000u, 0x00000001u));
			Assert::AreEqual(0x000000ffu, adds_rgba8_(0x000000deu, 0x000000abu));
			Assert::AreEqual(0x00000000u, adds_rgba8_(0x00000000u, 0x00000000u));
			Assert::AreEqual(0xffffffffu, adds_rgba8_(0xffffffffu, 0x01010101u));
		}

		TEST_METHOD(AddsRgba8Bmi)
		{
			Assert::AreEqual(0x00000000u, adds_rgba8_bmi(0x00000000u, 0x00000000u));
			Assert::AreEqual(0x01000001u, adds_rgba8_bmi(0x01000000u, 0x00000001u));
			Assert::AreEqual(0x000000ffu, adds_rgba8_bmi(0x000000deu, 0x000000abu));
			Assert::AreEqual(0x00000000u, adds_rgba8_bmi(0x00000000u, 0x00000000u));
			Assert::AreEqual(0xffffffffu, adds_rgba8_bmi(0xffffffffu, 0x01010101u));
		}

		TEST_METHOD(MidRgba8)
		{
			Assert::AreEqual(0x00000000u, mid_rgba8(0x00000000u, 0x00000000u));
			Assert::AreEqual(0x00000001u, mid_rgba8(0x00000001u, 0x00000001u));
			Assert::AreEqual(0x00000008u, mid_rgba8(0x00000010u, 0x00000000u));
			Assert::AreEqual(0x08080808u, mid_rgba8(0x00100010u, 0x10001000u));
			Assert::AreEqual(0xffffffffu, mid_rgba8(0xffffffffu, 0xffffffffu));
			Assert::AreEqual(0x80808080u, mid_rgba8(0xffffffffu, 0x00000000u));
		}

		TEST_METHOD(LerpRgba8)
		{
			Assert::AreEqual(0x00000000u, lerp_rgba8_(0x00000000u, 0xffffffff, 0x00));
			Assert::AreEqual(0xffffffffu, lerp_rgba8_(0x00000000u, 0xffffffff, 0x100));
			Assert::AreEqual(0x7f7f7f7fu, lerp_rgba8_(0x00000000u, 0xffffffff, 0x80));
			Assert::AreEqual(0x10203040u, lerp_rgba8_(0x00000000u, 0x20406080, 0x80));
		}

		TEST_METHOD(LerpRgba8Bmi)
		{
			Assert::AreEqual(0x00000000u, lerp_rgba8_bmi(0x00000000u, 0xffffffff, 0x00));
			Assert::AreEqual(0xffffffffu, lerp_rgba8_bmi(0x00000000u, 0xffffffff, 0x100));
			Assert::AreEqual(0x7f7f7f7fu, lerp_rgba8_bmi(0x00000000u, 0xffffffff, 0x80));
			Assert::AreEqual(0x10203040u, lerp_rgba8_bmi(0x00000000u, 0x20406080, 0x80));
		}

		TEST_METHOD(CvtRgba8Rgba10Bmi)
		{
			Assert::AreEqual(__rgba10(0, 0, 0, 0), cvt_rgba8_rgba10_bmi(0x00000000));
			Assert::AreEqual(__rgba10(3, 0, 0, 0), cvt_rgba8_rgba10_bmi(0xff000000));
			Assert::AreEqual(__rgba10(0, 0, 0, 0x3ff), cvt_rgba8_rgba10_bmi(0x000000ff));
			Assert::AreEqual(__rgba10(0, 0, 0x3ff, 0), cvt_rgba8_rgba10_bmi(0x0000ff00));
			Assert::AreEqual(__rgba10(0, 0x3ff, 0, 0), cvt_rgba8_rgba10_bmi(0x00ff0000));
			Assert::AreEqual(__rgba10(0, 0x202, 0, 0), cvt_rgba8_rgba10_bmi(0x00800000));
		}

		TEST_METHOD(CvtRgba10Rgba8Bmi)
		{
			Assert::AreEqual(0x00000000u, cvt_rgba10_rgba8_bmi(__rgba10(0, 0, 0, 0)));
			Assert::AreEqual(0xff000000u, cvt_rgba10_rgba8_bmi(__rgba10(3, 0, 0, 0)));
			Assert::AreEqual(0x000000ffu, cvt_rgba10_rgba8_bmi(__rgba10(0, 0, 0, 0x3ff)));
			Assert::AreEqual(0x0000ff00u, cvt_rgba10_rgba8_bmi(__rgba10(0, 0, 0x3ff, 0)));
			Assert::AreEqual(0x00ff0000u, cvt_rgba10_rgba8_bmi(__rgba10(0, 0x3ff, 0, 0)));
			Assert::AreEqual(0x00800000u, cvt_rgba10_rgba8_bmi(__rgba10(0, 0x202, 0, 0)));
		}
	};
};