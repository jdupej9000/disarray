#pragma once

#include <stdint.h>

void print_hex(const void* data, size_t size, size_t rowBytes = 16);
void print_identifier(uint32_t id);