
#include <iostream>
#include "TestMath.h"
#include "TestVFS.h"
#include "TestAssets.h"
#include "TestLog.h"


int main()
{
    std::cout << "Begining tests" << std::endl;

    std::cout << "Testing Math" << std::endl;
    test_math();
    std::cout << "Testing VFS" << std::endl;
    test_vfs();
    std::cout << "Testing Assets" << std::endl;
    test_assets_api();
    std::cout << "Testing Logs" << std::endl;
    test_log();
    std::cout << "Ending tests" << std::endl;
    std::cin.get();
}