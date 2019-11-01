#pragma once

#include <iostream>
#include <functional>

#include "KeyEvent.h"
#include "MouseEvent.h"
#include "WindowEvent.h"


//EventHandler is a basic function pointer to a const Event reference
template<typename E>
using EventHandler = std::function<void(const E&)>;

//Basic interface for an Event Manager
class EventManagerInterface
{
public:
    virtual ~EventManagerInterface() = default;
};


//EventManager implementation. Can add/remove handlers and dispatch specific events to them 
template<typename E>
class EventManager : public EventManagerInterface
{
private:
    std::vector<EventHandler<E>> handlers;

public:
    void add(const EventHandler<E> handler)
    {
        handlers.push_back(handler);
    }

    void remove(const EventHandler<E> handler)
    {
        auto it = std::find(handlers.begin(), handlers.end(), handler);
        if (it != handlers.end())
        {
            handlers.erase(it);
        }
    }

    void dispatch(const E& event)
    {
        if (event.handled)
            return;

        for (EventHandler<E> handler : handlers)
        {   
            handler(event);
            if (event.handled) 
                return;
        }
    }
};

class EventSystem
{
private:
    std::map<EventType, EventManagerInterface*> managers;

    template<typename E>
    EventManager<E>* ForEvent()
    {
        //Retrieve existing manager
        auto it = managers.find( E::getStaticType() );
        if (it != managers.end())
            return (EventManager<E>*) it->second;
        
        //Find unsuccessful, instanciate a new manager
        EventManager<E>* manager = new EventManager<E>();
        managers[ E::getStaticType() ] = manager;
        return manager;
    }

public:
    ~EventSystem()
    {
        //delete each manager
        for (auto entry : managers)
        {
            delete entry.second;
        }
    }

    template<typename E>
    void add(EventHandler<E> handler)
    {
        ForEvent<E>()->add(handler);
    }

    template<typename E>
    void remove(EventHandler<E> handler)
    {
        ForEvent<E>()->remove(handler);
    }

    template<typename E>
    void dispatch(const E& event)
    {
        ForEvent<E>()->dispatch(event);
    }
};