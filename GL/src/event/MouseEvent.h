#pragma once

#include "Event.h"

class MouseMovedEvent : public Event
{
public:
    const float x;
    const float y;

public:
    MouseMovedEvent(float x, float y) : x(x), y(y) {}

    EVENT_CLASS_TYPE(MouseMoved)
};

class MouseScrolledEvent : public Event
{
public:
    const float xOffset;
    const float yOffset;

public:
    MouseScrolledEvent(float xOffset, float yOffset) : xOffset(xOffset), yOffset(yOffset) {}

    EVENT_CLASS_TYPE(MouseScrolled)
};

class MouseButtonEvent : public Event
{
public:
    const int button;

protected:
    MouseButtonEvent(int button) : button(button) {}
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    EVENT_CLASS_TYPE(MouseButtonReleased)
};