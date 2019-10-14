#pragma once

#include "Event.h"

class KeyEvent : public Event
{
protected:
    int keycode;
    KeyEvent(int keycode) : keycode(keycode) {}

public:
    inline int getKeycode() const { return keycode; };
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

    EVENT_CLASS_TYPE(KeyPressed)
};

class KeyTypedEvent : public KeyEvent
{
public:
    KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

    EVENT_CLASS_TYPE(KeyPressed)
};

