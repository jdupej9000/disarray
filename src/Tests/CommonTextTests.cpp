#include "pch.h"
#include "CppunitTest.h"

#include <limits.h>

using namespace dsry::text;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(CommonText)
	{
	public:

		TEST_METHOD(FindText)
		{
			const char* pHay1 = "Zehn kleine Jaegermeister";
			Assert::IsNull(find_char(pHay1, '*'));
			Assert::AreEqual(4ll, find_char(pHay1, ' ') - pHay1);
			Assert::AreEqual(4ll, find_char(pHay1 + 1, ' ') - pHay1); // this or the one before will be unaligned
		}
	};
};