#include "Input.h"


vec2 Input::getMousePosition() const
{
    return mousePosition;
}

bool Input::isKeyPressed(unsigned int keycode) const
{
    return keycode < MAX_KEYS && keys[keycode];
}

bool Input::isButtonPressed(unsigned int button) const
{
    return button < MAX_BUTTONS && buttons[button];
}

void Input::setKeyPressed(unsigned int keycode, bool pressed)
{
    if (keycode < MAX_KEYS) keys[keycode] = pressed;
}

void Input::setButtonPressed(unsigned int button, bool pressed)
{
    if (button < MAX_BUTTONS) buttons[button] = pressed;
}

void Input::setMods(unsigned int mods)
{
    this->mods = mods;
}

void Input::setMousePosition(float x, float y)
{
    mousePosition.x = x;
    mousePosition.y = y;
}

void Input::setDragging(bool enabled)
{
    dragging = enabled;
}

void Input::clearKeys()
{
    for (int i = 0; i < MAX_KEYS; i++)
        keys[i] = false;
}

void Input::clearButtons()
{
    for (int i = 0; i < MAX_BUTTONS; i++)
        buttons[i] = false;
}