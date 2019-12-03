#pragma once

#include <stdint.h>

//http://prng.di.unimi.it/xoshiro256plus.c
class Random 
{
private:
    uint64_t seed[4]; // 256bit seed

    uint64_t next();
public:
    /**
     * Seeds the Random generator with the specified 256bit seed
     * @param s0 First quarter of the seed (lsb)
     * @param s1 Second quarter of the seed (lsb)
     * @param s2 Third quarter of the seed (msb)
     * @param s3 Fourth quarter of the seed (msb)
     * 
    */
    void Seed(uint64_t s0, uint64_t s1, uint64_t s2, uint64_t s3);

    /**
     * Retrieves a random number in the range [low; high) 
     * @param low lower limit (inclusive)
     * @param high upper limit (exclu)
     * @return a random number in range [low; high) using the next seed
    */
    int64_t Integer(int64_t low, int64_t high);

    /**
     * Retrieves a random number in the range [low; high) 
     * @param low lower limit (inclusive)
     * @param high upper limit (exclu)
     * @return a random number in range [low; high) using the next seed
    */
    float   Float(float low, float high);

    /**
     * Retrieves a random number in the range [low; high) 
     * @param low lower limit (inclusive)
     * @param high upper limit (exclu)
     * @return a random number in range [low; high) using the next seed
    */
    double  Double(double low, double high);

    //TODO : non uniform distributions ?
};