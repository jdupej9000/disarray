#include <stdint.h>
#include <intrin.h>

namespace dsry::text
{

	const char* find_char(const char* pszText, char needle)
	{
		// https://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
		const char* p = pszText;

		while ((size_t)p & ~0x7f)
		{
			if (*p == needle) return p;
			if (!*p) return nullptr;
			p++;
		}

		const uint64_t* p8 = (const uint64_t*)p;
		uint64_t nrep = (uint64_t)needle * 0x0101010101010101ull;

		while (1)
		{
			uint64_t v = *p8;
			uint64_t hasz = (v - 0x0101010101010101ull) & ~v & 0x8080808080808080ull;
			v ^= nrep;
			uint64_t hasn = (v - 0x0101010101010101ull) & ~v & 0x8080808080808080ull;

			if (hasz | hasn)
			{
				if (hasz < hasn) return nullptr; // found zero before needle
				else return (const char*)p8 + ((_tzcnt_u64(hasz | hasn) + 1) >> 3) - 1;
			}

			p8++;
		}
	}
};