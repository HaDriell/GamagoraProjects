#pragma once

#include <functional>

#include "KeyEvent.h"
#include "MouseEvent.h"
#include "WindowEvent.h"

template<typename T>
using EventHandler = std::function<void(T&)>; 

template<typename T>
using EventHandlerList = std::vector<EventHandler<T>>;

class EventSystem
{
private:
    std::map<EventType, void*> listeners;

    //Finds or builds
    template<typename E>
    EventHandlerList<E>* ForEvent()
    {
        EventHandlerList<E>* list = nullptr;
        auto entry = listeners.find( E::getStaticType() );
        if (entry == listeners.end())
        {
            list = new EventHandlerList<E>();
            listeners[ E::getStaticType() ] = (void*) list;
        }
        else
        {
            list = (EventHandlerList<E>*) entry->second;
        }
        return list;
    }

public:
    ~EventSystem()
    {
        //Window Events cleanup
        delete ForEvent<WindowClosedEvent>();
        delete ForEvent<WindowResizedEvent>();
        delete ForEvent<WindowGainedFocusEvent>();
        delete ForEvent<WindowLostFocusEvent>();
        delete ForEvent<WindowMovedEvent>();

        //Key Events cleanup
        delete ForEvent<KeyPressedEvent>();
        delete ForEvent<KeyReleasedEvent>();
        delete ForEvent<KeyTypedEvent>();
        
        //Mouse Events cleanup
        delete ForEvent<MouseButtonPressedEvent>();
        delete ForEvent<MouseButtonReleasedEvent>();
        delete ForEvent<MouseMovedEvent>();
        delete ForEvent<MouseScrolledEvent>();
    }

    template<typename E>
    void add(EventHandler<E> handler)
    {
        ForEvent<E>()->push_back(handler);
    }

    template<typename E>
    void dispatch(E& event)
    {
        if (!event.handled)
        {
            for (EventHandler<E>& handler : *ForEvent<E>())
            {
                handler(event);
                if (event.handled) 
                    return;
            }
        }
    }
};