#pragma once

#include "Entity.h"
#include "Timer.h"

class Poop : public Entity
{
private:
    Timer decayTimer;

public:
    Poop(vec2 position) : Entity(POOP, position) {}

    virtual void event(Game& game, const sf::Event& event) override;
    virtual void udpate(Game& game) override;
    virtual void render(Game& game) override;
};