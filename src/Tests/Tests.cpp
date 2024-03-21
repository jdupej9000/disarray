#include "pch.h"
#include "CppUnitTest.h"

#include <limits.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(CommonEncoding)
	{
	public:
		
		TEST_METHOD(EncodeMortonBmi32)
		{
			Assert::AreEqual(0x0U, encode_morton_bmi((uint16_t)0x0, (uint16_t)0x0));
			Assert::AreEqual(0x1U, encode_morton_bmi((uint16_t)0x1, (uint16_t)0x0));
			Assert::AreEqual(0x2U, encode_morton_bmi((uint16_t)0x0, (uint16_t)0x1));
			
		}

		TEST_METHOD(EncodeMortonBmi64)
		{
			Assert::AreEqual(0x0ULL, encode_morton_bmi((uint32_t)0x0, (uint32_t)0x0));
			Assert::AreEqual(0x1ULL, encode_morton_bmi((uint32_t)0x1, (uint32_t)0x0));
			Assert::AreEqual(0x2ULL, encode_morton_bmi((uint32_t)0x0, (uint32_t)0x1));
		}

		TEST_METHOD(EncodeZigZagBmi32)
		{
			Assert::AreEqual(0x0U, encode_zigzag_bmi(0));
			Assert::AreEqual(0x1U, encode_zigzag_bmi(-1));
			Assert::AreEqual(0xfffffffeU, encode_zigzag_bmi(INT_MAX));
			Assert::AreEqual(0xffffffffU, encode_zigzag_bmi(INT_MIN));
		}

		TEST_METHOD(EncodeZigZagBmi64)
		{
			Assert::AreEqual(0x0ULL, encode_zigzag_bmi(0LL));
			Assert::AreEqual(0x1ULL, encode_zigzag_bmi(-1LL));
			Assert::AreEqual(0xfffffffffffffffeULL, encode_zigzag_bmi(INT64_MAX));
			Assert::AreEqual(0xffffffffffffffffULL, encode_zigzag_bmi(INT64_MIN));
		}

		TEST_METHOD(EncodeLeb128Bmi32)
		{
			uint8_t buff[8];

			Assert::AreEqual(1, encode_leb128_bmi(buff, 0U));
			Assert::AreEqual((uint8_t)0, buff[0]);

			Assert::AreEqual(1, encode_leb128_bmi(buff, 1U));
			Assert::AreEqual((uint8_t)1, buff[0]);

			Assert::AreEqual(2, encode_leb128_bmi(buff, 128U));
			Assert::AreEqual((uint8_t)0x80, buff[0]);
			Assert::AreEqual((uint8_t)0x01, buff[1]);

			Assert::AreEqual(5, encode_leb128_bmi(buff, UINT32_MAX));
			Assert::AreEqual((uint8_t)0xff, buff[0]);
			Assert::AreEqual((uint8_t)0xff, buff[1]);
			Assert::AreEqual((uint8_t)0xff, buff[2]);
			Assert::AreEqual((uint8_t)0xff, buff[3]);
			Assert::AreEqual((uint8_t)0x0f, buff[4]);
		}

		TEST_METHOD(EncodeLeb128Bmi64)
		{
			uint8_t buff[16];

			Assert::AreEqual(1, encode_leb128_bmi(buff, 0ULL));
			Assert::AreEqual((uint8_t)0, buff[0]);

			Assert::AreEqual(1, encode_leb128_bmi(buff, 1ULL));
			Assert::AreEqual((uint8_t)1, buff[0]);

			Assert::AreEqual(2, encode_leb128_bmi(buff, 128ULL));
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
	};
}
