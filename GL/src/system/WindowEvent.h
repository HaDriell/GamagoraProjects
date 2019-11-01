#pragma once

#include "Event.h"

class WindowResizedEvent : public Event
{
private:
    unsigned int width;
    unsigned int height;

public:
    inline unsigned int getWidth() const { return width; }
    inline unsigned int getHeight() const { return height; }

    WindowResizedEvent(unsigned int width, unsigned int height) : width(width), height(height) {}

    EVENT_CLASS_TYPE(WindowResized)
};

class WindowClosedEvent : public Event
{
public:
    WindowClosedEvent() {}

    EVENT_CLASS_TYPE(WindowClosed)
};

class WindowGainedFocusEvent : public Event
{
public:
    WindowGainedFocusEvent() {}

    EVENT_CLASS_TYPE(WindowGainedFocus)
};

class WindowLostFocusEvent : public Event
{
public:
    WindowLostFocusEvent() {}

    EVENT_CLASS_TYPE(WindowLostFocus)
};

class WindowMovedEvent : public Event
{
private:
    unsigned int x;
    unsigned int y;

public:
    inline unsigned int getX() const { return x; }
    inline unsigned int getY() const { return y; }

    WindowMovedEvent(unsigned int x, unsigned int y) : x(x), y(y) {}

    EVENT_CLASS_TYPE(WindowMoved)
};