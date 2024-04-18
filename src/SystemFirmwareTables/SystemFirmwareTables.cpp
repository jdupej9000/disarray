// SystemFirmwareTables.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "utils.h"
#include <iostream>
#include <Windows.h>

void enum_tables(DWORD tableSignature);

using namespace std;

int main()
{
    //enum_tables('ACPI');
//    enum_tables('RSMB');
    enum_tables('FIRM');
}



void enum_tables(DWORD tableSignature)
{
    size_t numBytes = EnumSystemFirmwareTables(tableSignature, NULL, 0);
    uint8_t* raw = new uint8_t[numBytes];

    UINT ret = EnumSystemFirmwareTables(tableSignature, raw, numBytes);
    const DWORD* tables = (const DWORD*)raw;
    size_t numTables = numBytes / 4;

    for (size_t i = 0; i < numTables; i++)
    {
        size_t tableSize = GetSystemFirmwareTable(tableSignature, tables[i], NULL, 0);
        uint8_t* tableRaw = new uint8_t[tableSize];

        GetSystemFirmwareTable(tableSignature, tables[i], tableRaw, tableSize);

        print_identifier(tableSignature);
        cout << "::";
        print_identifier(tables[i]);
        cout << endl;
        print_hex(tableRaw, tableSize);

        getchar();

        delete[] tableRaw;
    }


    delete[] raw;
}