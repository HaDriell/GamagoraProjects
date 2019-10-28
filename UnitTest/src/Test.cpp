
#include <iostream>
#include "TestMath.h"
#include "TestVFS.h"
#include "TestAssetManager.h"


int main()
{
    std::cout << "Begining tests" << std::endl;
    test_math();
    test_vfs();
    test_asset_manager();

    std::cout << "Ending tests" << std::endl;
    std::cin.get();
}