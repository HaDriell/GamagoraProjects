#pragma once

#include "vec2.h"
#include "Game.h"

enum EntityType
{
    SYSTEM,
    GAMAGOCHI,
    POOP,
    FRUIT
};

struct Entity
{
    vec2 position;
    const EntityType type;

//In Game Update & Render "callbacks"
public:
    Entity(EntityType type, vec2 position = vec2());
    virtual ~Entity();
    virtual void event(Game& game, const sf::Event& event) = 0;
    virtual void udpate(Game& game) = 0;
    virtual void render(Game& game) = 0;


//In Game Destruction API
public:
    void destroy();
    bool isDestroyed();
private:
    bool destroyed = false;
};