#include "utils.h"
#include <iostream>
#include <iomanip>

using namespace std;

void print_hex(const void* data, size_t size, size_t rowBytes)
{
	for (size_t offs = 0; offs < size; offs += rowBytes)
	{
		const uint8_t* row = (const uint8_t*)data + offs;
		size_t rowLen = size - offs;
		if (rowLen > rowBytes) rowLen = rowBytes;

		cout << hex << setw(4) << setfill(' ') << offs << " | ";

		for (size_t i = 0; i < rowLen; i++)
			cout << hex << setw(2) << setfill('0') << (uint32_t)row[i] << ' ';

		for (size_t i = rowLen; i < rowBytes; i++)
			cout << "   ";

		cout << " | ";

		for (size_t i = 0; i < rowLen; i++)
		{
			char ch = row[i];

			if (isgraph(ch))
				cout << ch;
			else
				cout << '.';
		}

		cout << endl;
	}
}

void print_identifier(uint32_t id)
{
	char data[5];
	data[0] = (id >> 24) & 0xff;
	data[1] = (id >> 16) & 0xff;
	data[2] = (id >> 8) & 0xff;
	data[3] = (id) & 0xff;
	data[4] = '\0';

	cout << data << " (" << setw(8) << hex << id << ")";
}