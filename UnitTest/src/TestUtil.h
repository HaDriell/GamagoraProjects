#pragma once

#include <math.h>
#include <cstdlib>
#include <assert.h>
#include <iostream>

#define ASSERT_ESPILON 1e-6

//Default Assert
#define ASSERT_TRUE(cond)                       \
        do {                                    \
            if (!(cond))                        \
            {                                   \
                std::cerr << "Failed assertion "\
                << #cond << " @" << __FILE__    \
                << "(" <<  __LINE__ << ")"      \
                << std::endl;                   \
            }                                   \
        } while(0);                             \

//Inverse Assert
#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))

//Global Assertion
#define ASSERT_EQ(a, b) ASSERT_TRUE((a) == (b))

//Assertion for Floating point values
#define ASSERT_EQ_F(a, b) ASSERT_TRUE((std::abs((a) - (b)) < ASSERT_ESPILON))

