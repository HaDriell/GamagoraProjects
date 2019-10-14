
#include <iostream>
#include "TestMath.h"
#include "TestGL.h"

// All broken
// #include "TestMath.h"
// #include "TestAI.h"
// #include "TestRT.h"


int main()
{
    std::cout << "Begining tests" << std::endl;
    test_math();

    test_gl();
    std::cout << "Ending tests" << std::endl;
    std::cin.get();
}