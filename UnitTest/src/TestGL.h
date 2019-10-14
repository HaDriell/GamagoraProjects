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

    w.events().add<KeyPressedEvent>([&](const KeyPressedEvent& event)
    {
        if (event.keycode == KEY_ESCAPE)
        {
            w.close();
        }
        
        if (event.keycode == KEY_C)
        {
            s.compileFile("res/example.glsl");
        }

        if (event.keycode == KEY_T)
        {
            t.loadFile("res/metal.png");
        }
    });

    while (!w.shouldClose())
    {
        w.update();
    }
}