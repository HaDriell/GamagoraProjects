#pragma once

#include "maths.h"
#include "Entity.h"



class TerrainEditor : public Entity
{
    int mode;

public:
    TerrainEditor() : Entity(SYSTEM), mode(0) {}
    ~TerrainEditor() {}
    
    virtual void event(Game& game, const sf::Event& event) override;
    virtual void udpate(Game& game) override;
    virtual void render(Game& game) override;

};