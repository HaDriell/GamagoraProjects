#pragma once

#include "Event.h"

class MouseMovedEvent : public Event
{
private:
    unsigned int x;
    unsigned int y;

public:
    MouseMovedEvent(unsigned int x, unsigned int y) : x(x), y(y) {}

    EVENT_CLASS_TYPE(MouseMoved)
};

class MouseScrolledEvent : public Event
{
private:
    float xOffset;
    float yOffset;

public:
    MouseScrolledEvent(float xOffset, float yOffset) : xOffset(xOffset), yOffset(yOffset) {}

    EVENT_CLASS_TYPE(MouseScrolled)
};

class MouseButtonEvent : public Event
{
protected:
    int button;
    MouseButtonEvent(int button) : button(button) {}

public:
    inline int getButton() const { return button; }
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