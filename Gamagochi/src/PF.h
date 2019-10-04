#pragma once

#include "Entity.h"
#include <vector>


class PF : public Entity
{
private:
    vec2 start;
    vec2 end;
    std::vector<vec2> path;


    void computePath(const Game& game);

public:

    PF();
    virtual ~PF();
    
    virtual void event(const Game& game, const sf::Event& event);
    virtual void udpate(const Game& game);
    virtual void render(const Game& game);
};