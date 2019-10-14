#include "TestUtil.h"
#include "Graphics.h"

//Declarations

void test_gl();


//Implementations

void test_gl()
{
    Window w = Window();
    while (!w.shouldClose())
    {
        w.update();
    }
}