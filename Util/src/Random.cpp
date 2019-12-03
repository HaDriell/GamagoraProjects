#include "Random.h"

#include <limits>

static inline uint64_t rotl(const uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}

uint64_t Random::next()
{
    const uint64_t result = seed[0] + seed[3]; 
    const uint64_t t = seed[1] << 17;

    seed[2] ^= seed[0];
    seed[3] ^= seed[1];
    seed[1] ^= seed[2];
    seed[0] ^= seed[3];

    seed[2] ^= t;

    seed[3] = rotl(seed[3], 45);

    return result;
}

void Random::Seed(uint64_t s0, uint64_t s1, uint64_t s2, uint64_t s3)
{
    seed[0] = s0;
    seed[1] = s1;
    seed[2] = s2;
    seed[3] = s3;
}


/* 2 implementations from a paper

uint64_t java ( uint64_t s, uint64_t (* random64 )( void )) {
uint64_t x = random64 ();
uint64_t r = x % s;
while (x - r > UINT64_MAX - s + 1) {
x = random64 ();
r = x % s;
}
return r;
}

uint64_t nearlydivisionless ( uint64_t s, uint64_t (* random64 )( void )) {
uint64_t x = random64 ();
__uint128_t m = ( __uint128_t ) x * ( __uint128_t ) s;
uint64_t l = ( uint64_t ) m;
if (l < s) {
uint64_t t = -s % s;
while (l < t) {
x = random64 ();
m = ( __uint128_t ) x * ( __uint128_t ) s;
l = ( uint64_t ) m;
}
}
return m >> 64;
}
*/

int64_t Random::Integer(int64_t low, int64_t high)
{
    uint64_t modulo = high - low;
    uint64_t result, random;

    do {
        random = next();
        result = random % modulo; 
    } while (random - result > std::numeric_limits<uint64_t>::max() - modulo + 1);

    return result;
}

float Random::Float(float low, float high)
{
    float t = next() / (float) std::numeric_limits<uint64_t>::max();
    return low + (t * (high - low));
}

double Random::Double(double low, double high)
{
    double t = next() / (double) std::numeric_limits<uint64_t>::max();
    return low + (t * (high - low));
}