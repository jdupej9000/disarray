#include "pch.h"
#include "CppunitTest.h"

#include <limits.h>

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
			Assert::AreEqual(0x00000000u, adds_rgba8(0x00000000u, 0x00000000u));
			Assert::AreEqual(0x01000001u, adds_rgba8(0x01000000u, 0x00000001u));
			Assert::AreEqual(0x000000ffu, adds_rgba8(0x000000deu, 0x000000abu));
			Assert::AreEqual(0x00000000u, adds_rgba8(0x00000000u, 0x00000000u));
			Assert::AreEqual(0xffffffffu, adds_rgba8(0xffffffffu, 0x01010101u));
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
			Assert::AreEqual(0x00000000u, lerp_rgba8(0x00000000u, 0xffffffff, 0x00));
			Assert::AreEqual(0xffffffffu, lerp_rgba8(0x00000000u, 0xffffffff, 0x100));
			Assert::AreEqual(0x7f7f7f7fu, lerp_rgba8(0x00000000u, 0xffffffff, 0x80));
			Assert::AreEqual(0x10203040u, lerp_rgba8(0x00000000u, 0x20406080, 0x80));
		}

		TEST_METHOD(LerpRgba8Bmi)
		{
			Assert::AreEqual(0x00000000u, lerp_rgba8_bmi(0x00000000u, 0xffffffff, 0x00));
			Assert::AreEqual(0xffffffffu, lerp_rgba8_bmi(0x00000000u, 0xffffffff, 0x100));
			Assert::AreEqual(0x7f7f7f7fu, lerp_rgba8_bmi(0x00000000u, 0xffffffff, 0x80));
			Assert::AreEqual(0x10203040u, lerp_rgba8_bmi(0x00000000u, 0x20406080, 0x80));
		}
	};

	TEST_CLASS(CommonEncoding)
	{
	public:
		
		TEST_METHOD(EncodeMortonBmi32)
		{
			Assert::AreEqual(0x0u, encode_morton_bmi((uint16_t)0x0, (uint16_t)0x0));
			Assert::AreEqual(0x1u, encode_morton_bmi((uint16_t)0x1, (uint16_t)0x0));
			Assert::AreEqual(0x2u, encode_morton_bmi((uint16_t)0x0, (uint16_t)0x1));
		}

		TEST_METHOD(DecodeMortonBmi32)
		{
			uint16_t a=0xdead, b=0xbeef;
			decode_morton_bmi(0x0, a, b);
			Assert::AreEqual((uint16_t)0x0u, a);
			Assert::AreEqual((uint16_t)0x0u, b);

			decode_morton_bmi(0x1, a, b);
			Assert::AreEqual((uint16_t)0x1u, a);
			Assert::AreEqual((uint16_t)0x0u, b);

			decode_morton_bmi(0x2, a, b);
			Assert::AreEqual((uint16_t)0x0u, a);
			Assert::AreEqual((uint16_t)0x1u, b);
		}

		TEST_METHOD(EncodeMortonBmi64)
		{
			Assert::AreEqual(0x0ull, encode_morton_bmi((uint32_t)0x0, (uint32_t)0x0));
			Assert::AreEqual(0x1ull, encode_morton_bmi((uint32_t)0x1, (uint32_t)0x0));
			Assert::AreEqual(0x2ull, encode_morton_bmi((uint32_t)0x0, (uint32_t)0x1));
		}

		TEST_METHOD(DecodeMortonBmi64)
		{
			uint32_t a = 0xdeadbeef, b = 0xbeefdead;
			decode_morton_bmi(0x0ull, a, b);
			Assert::AreEqual(0x0u, a);
			Assert::AreEqual(0x0u, b);

			decode_morton_bmi(0x1ull, a, b);
			Assert::AreEqual(0x1u, a);
			Assert::AreEqual(0x0u, b);

			decode_morton_bmi(0x2ull, a, b);
			Assert::AreEqual(0x0u, a);
			Assert::AreEqual(0x1u, b);
		}

		TEST_METHOD(EncodeZigZagBmi32)
		{
			Assert::AreEqual(0x0u, encode_zigzag_bmi(0));
			Assert::AreEqual(0x1u, encode_zigzag_bmi(-1));
			Assert::AreEqual(0x2u, encode_zigzag_bmi(1));
			Assert::AreEqual(0xfffffffeu, encode_zigzag_bmi(INT_MAX));
			Assert::AreEqual(0xffffffffu, encode_zigzag_bmi(INT_MIN));
		}

		TEST_METHOD(DecodeZigZagBmi32)
		{
			Assert::AreEqual(0, decode_zigzag_bmi(0x0u));
			Assert::AreEqual(-1, decode_zigzag_bmi(0x1u));
			Assert::AreEqual(1, decode_zigzag_bmi(0x2u));
			Assert::AreEqual(INT_MAX, decode_zigzag_bmi(0xfffffffeu));
			Assert::AreEqual(INT_MIN, decode_zigzag_bmi(0xffffffffu));
		}

		TEST_METHOD(EncodeZigZagBmi64)
		{
			Assert::AreEqual(0x0ull, encode_zigzag_bmi(0ll));
			Assert::AreEqual(0x1ull, encode_zigzag_bmi(-1ll));
			Assert::AreEqual(0x2ull, encode_zigzag_bmi(1ll));
			Assert::AreEqual(0xfffffffffffffffeull, encode_zigzag_bmi(INT64_MAX));
			Assert::AreEqual(0xffffffffffffffffull, encode_zigzag_bmi(INT64_MIN));
		}

		TEST_METHOD(DecodeZigZagBmi64)
		{
			Assert::AreEqual(0ll, decode_zigzag_bmi(0x0ull));
			Assert::AreEqual(-1ll, decode_zigzag_bmi(0x1ull));
			Assert::AreEqual(1ll, decode_zigzag_bmi(0x2ull));
			Assert::AreEqual(INT64_MAX, decode_zigzag_bmi(0xfffffffffffffffeull));
			Assert::AreEqual(INT64_MIN, decode_zigzag_bmi(0xffffffffffffffffull));
		}

		TEST_METHOD(EncodeLeb128Bmi32)
		{
			uint8_t buff[8];

			Assert::AreEqual(1, encode_leb128_bmi(buff, 0u));
			Assert::AreEqual((uint8_t)0, buff[0]);

			Assert::AreEqual(1, encode_leb128_bmi(buff, 1u));
			Assert::AreEqual((uint8_t)1, buff[0]);

			Assert::AreEqual(2, encode_leb128_bmi(buff, 128u));
			Assert::AreEqual((uint8_t)0x80, buff[0]);
			Assert::AreEqual((uint8_t)0x01, buff[1]);

			Assert::AreEqual(5, encode_leb128_bmi(buff, UINT32_MAX));
			Assert::AreEqual((uint8_t)0xff, buff[0]);
			Assert::AreEqual((uint8_t)0xff, buff[1]);
			Assert::AreEqual((uint8_t)0xff, buff[2]);
			Assert::AreEqual((uint8_t)0xff, buff[3]);
			Assert::AreEqual((uint8_t)0x0f, buff[4]);
		}

		TEST_METHOD(DecodeLeb128Bmi32)
		{
			uint8_t buff0[8]{ 0x0, 0xde, 0xad, 0xbe, 0xef, 0x0, 0x0, 0x0 };
			uint8_t buff1[8]{ 0x1, 0xde, 0xad, 0xbe, 0xef, 0x0, 0x0, 0x0 };
			uint8_t buff2[8]{ 0x80, 0x01, 0xde, 0xad, 0xbe, 0xef, 0x0, 0x0 };
			uint8_t buff3[8]{ 0xff, 0xff, 0xff, 0xff, 0x0f, 0xde, 0xad, 0xbe };

			uint32_t x = 0xdeadbeef;
			Assert::AreEqual(1, decode_leb128_bmi(buff0, x));
			Assert::AreEqual(0u, x);

			Assert::AreEqual(1, decode_leb128_bmi(buff1, x));
			Assert::AreEqual(1u, x);

			Assert::AreEqual(2, decode_leb128_bmi(buff2, x));
			Assert::AreEqual(128u, x);

			Assert::AreEqual(5, decode_leb128_bmi(buff3, x));
			Assert::AreEqual(UINT32_MAX, x);
		}

		TEST_METHOD(EncodeLeb128Bmi64)
		{
			uint8_t buff[16];

			Assert::AreEqual(1, encode_leb128_bmi(buff, 0ull));
			Assert::AreEqual((uint8_t)0, buff[0]);

			Assert::AreEqual(1, encode_leb128_bmi(buff, 1ull));
			Assert::AreEqual((uint8_t)1, buff[0]);

			Assert::AreEqual(2, encode_leb128_bmi(buff, 128ull));
			Assert::AreEqual((uint8_t)0x80, buff[0]);
			Assert::AreEqual((uint8_t)0x01, buff[1]);

			Assert::AreEqual(10, encode_leb128_bmi(buff, UINT64_MAX));
			Assert::AreEqual((uint8_t)0xff, buff[0]);
			Assert::AreEqual((uint8_t)0xff, buff[1]);
			Assert::AreEqual((uint8_t)0xff, buff[2]);
			Assert::AreEqual((uint8_t)0xff, buff[3]);
			Assert::AreEqual((uint8_t)0xff, buff[4]);
			Assert::AreEqual((uint8_t)0xff, buff[5]);
			Assert::AreEqual((uint8_t)0xff, buff[6]);
			Assert::AreEqual((uint8_t)0xff, buff[7]);
			Assert::AreEqual((uint8_t)0xff, buff[8]);
			Assert::AreEqual((uint8_t)0x01, buff[9]);
		}

		TEST_METHOD(DecodeLeb128Bmi64)
		{
			uint8_t buff0[16]{ 0x0, 0xde, 0xad, 0xbe, 0xef, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
			uint8_t buff1[16]{ 0x1, 0xde, 0xad, 0xbe, 0xef, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
			uint8_t buff2[16]{ 0x80, 0x01, 0xde, 0xad, 0xbe, 0xef, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
			uint8_t buff3[16]{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xde, 0xad, 0xbe, 0xef, 0x0, 0x0 };
			

			uint64_t x = 0xdeadbeef;
			Assert::AreEqual(1, decode_leb128_bmi(buff0, x));
			Assert::AreEqual(0ull, x);

			Assert::AreEqual(1, decode_leb128_bmi(buff1, x));
			Assert::AreEqual(1ull, x);
			
			Assert::AreEqual(2, decode_leb128_bmi(buff2, x));
			Assert::AreEqual(128ull, x);
			
			Assert::AreEqual(10, decode_leb128_bmi(buff3, x));
			Assert::AreEqual(UINT64_MAX, x);
		}
	};
}
