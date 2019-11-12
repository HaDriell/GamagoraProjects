#pragma once

#include <string>
#include <stddef.h>

const unsigned int FNV1A_PRIME  = 0x01000193;
const unsigned int FNV1A_SEED   = 0x811C9DC5;

inline constexpr unsigned int fnv1a(unsigned char byte, unsigned int hash = FNV1A_SEED)
{
    return (byte ^ hash) * FNV1A_PRIME;
}

unsigned int fnv1a(const void* data, size_t size, unsigned int hash = FNV1A_SEED);
unsigned int fnv1a(float number, unsigned int hash = FNV1A_SEED);
unsigned int fnv1a(double number, unsigned int hash = FNV1A_SEED);
unsigned int fnv1a(const char* text, unsigned int hash = FNV1A_SEED);
unsigned int fnv1a(const std::string& text, unsigned int hash = FNV1A_SEED);