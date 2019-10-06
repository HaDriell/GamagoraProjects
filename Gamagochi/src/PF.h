#pragma once

#include "Entity.h"
#include <vector>


class PF : public Entity
{
private:
    vec2 start;
    vec2 end;

    vec2 hoveredTile;
    bool astar;

    std::vector<vec2> path;

    void computePath(const Game& game);

public:

    PF();
    virtual ~PF();

    void use_astar();
    void use_dijkstra();
    
    virtual void event(Game& game, const sf::Event& event) override;
    virtual void udpate(Game& game) override;
    virtual void render(Game& game) override;
};