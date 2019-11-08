#pragma once

#include "../Common.h"

#define MAX_KEYS    1024
#define MAX_BUTTONS 32

class Window;

class Input
{
private:
    bool    keys[MAX_KEYS];

    bool    buttons[MAX_BUTTONS];
    bool    dragging;
    int     mods;
    vec2    mousePosition;

public:
    void setKeyPressed(unsigned int keycode, bool pressed);
    void setButtonPressed(unsigned int button, bool pressed);
    void setMods(unsigned int mods);
    void setDragging(bool enabled);
    void setMousePosition(float x, float y);

    bool isModPressed(unsigned int mask) const;
    bool isKeyPressed(unsigned int keycode) const;
    bool isButtonPressed(unsigned int button) const;
    vec2 getMousePosition() const;


    void clearKeys();
    void clearButtons();
};