#pragma once


#include "Entity.h"

enum Goal
{
    Idle,
    LookingForFood,
    ChasingFood,
    EatingFood
};

class Gamagochi : public Entity
{
    Timer hungerCooldown;
    Timer movingCooldown;
    Goal goal = Idle;
    vec2 target;

public:
    Gamagochi(vec2 position) : Entity(GAMAGOCHI, position) {}

    virtual void event(Game& game, const sf::Event& event) override;
    virtual void udpate(Game& game) override;
    virtual void render(Game& game) override;
};