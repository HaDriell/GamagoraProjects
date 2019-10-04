#pragma once

#include "vec2.h"
#include "Game.h"

enum EntityType
{
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
    Entity(EntityType type);
    virtual ~Entity();
    virtual void event(const Game& game, const sf::Event& event) = 0;
    virtual void udpate(const Game& game) = 0;
    virtual void render(const Game& game) = 0;


//In Game Destruction API
public:
    void destroy();
    bool isDestroyed();
private:
    bool destroyed = false;
};