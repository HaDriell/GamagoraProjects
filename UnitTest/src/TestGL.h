#include "TestUtil.h"
#include <Events.h>
#include <Graphics.h>

//Declarations

void test_gl();


//Implementation

void test_event_system()
{
    EventSystem events;
}

void test_gl()
{
    Window w = Window();
    Shader s;
    Texture t;
    // Mesh m;

    w.events().add<KeyPressedEvent>([&](const KeyPressedEvent& event)
    {
        if (event.keycode == KEY_ESCAPE)
        {
            w.close();
        }
        
        if (event.keycode == KEY_S)
        {
            std::cout << "Compiling Shader" << std::endl;
            s.compileFile("res/example.glsl");
            std::cout << "Compiling Shader Done" << std::endl;
        }

        if (event.keycode == KEY_T)
        {
            std::cout << "Loading Texture" << std::endl;
            t.loadFile("res/metal.png");
            std::cout << "Loading Texture Done" << std::endl;
        }

        // if (event.keycode == KEY_M)
        // {
        //     std::cout << "Loading Mesh" << std::endl;
        //     m.loadFromOBJ("res/bunny.obj");
        //     std::cout << "Loading Mesh Done" << std::endl;
        // }

        if (event.keycode == KEY_R)
        {
            // m.draw(s);
        }
    });

    while (!w.shouldClose())
    {
        w.update();
    }
}