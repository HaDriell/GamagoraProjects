#pragma once

#include "Event.h"

class KeyEvent : public Event
{
public:
    const int keycode;

protected:
    KeyEvent(int keycode) : keycode(keycode) {}
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int keycode) : KeyEvent(keycode) {}

    EVENT_CLASS_TYPE(KeyPressed)
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
    KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

    EVENT_CLASS_TYPE(KeyTyped)
};

