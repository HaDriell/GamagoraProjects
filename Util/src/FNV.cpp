#include "FNV.h"


unsigned int fnv1a(const void* data, size_t size, unsigned int hash)
{
    const unsigned char* ptr = (const unsigned char*) data;
    const unsigned char* end = ptr + size;
    while (ptr != end)
    {
        hash = fnv1a(*ptr++, hash);
    }
    return hash;
}

unsigned int fnv1a(float number, unsigned int hash)
{
    return fnv1a(&number, sizeof(float), hash);
}

unsigned int fnv1a(double number, unsigned int hash)
{
    return fnv1a(&number, sizeof(double), hash);
}

unsigned int fnv1a(const char* text, unsigned int hash)
{
    while (*text)
        hash = fnv1a((unsigned char)*text++, hash);
    return hash;
}

unsigned int fnv1a(const std::string& text, unsigned int hash)
{
    return fnv1a(text.c_str(), text.length(), hash);
}