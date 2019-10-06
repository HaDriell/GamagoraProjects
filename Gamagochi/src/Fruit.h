#pragma once

#include "Entity.h"
#include "Timer.h"

class Fruit : public Entity
{

public:
    Fruit(vec2 position) : Entity(FRUIT, position) {}

    virtual void event(Game& game, const sf::Event& event) override;
    virtual void udpate(Game& game) override;
    virtual void render(Game& game) override;
};