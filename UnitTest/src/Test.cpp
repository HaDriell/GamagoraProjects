
#include <iostream>
#include "TestMath.h"
#include "TestVFS.h"
#include "TestAssets.h"
#include "TestLog.h"


int main()
{
    std::cout << "Begining tests" << std::endl;
    test_math();
    test_vfs();
    test_assets_api();
    test_log();

    std::cout << "Ending tests" << std::endl;
    std::cin.get();
}