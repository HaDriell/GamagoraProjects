#pragma once

#include <map>
#include <vector>

enum class EventType
{
    None = 0,
    WindowClosed, WindowResized, WindowGainedFocus, WindowLostFocus, WindowMoved,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

#define EVENT_CLASS_TYPE(type)                      \
    static EventType getStaticType()                \
    {                                               \
        return EventType::type;                     \
    }                                               \
    virtual EventType getEventType() const override \
    {                                               \
        return getStaticType();                     \
    }                                               \

// EVENT_CLASS_TYPE //

class Event
{
public:
    bool handled = false;

    virtual EventType getEventType() const = 0;
};

