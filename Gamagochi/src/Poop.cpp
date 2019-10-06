#include "Poop.h"




void Poop::event(Game& game, const sf::Event& event)
{
}

void Poop::udpate(Game& game)
{
    //Despawn after a while
    if (decayTimer.elapsed() > 10)
        destroy();
}

void Poop::render(Game& game)
{
    sf::Sprite tile = sf::Sprite();
    tile.setScale(2, 2);
    vec2 tposition = game.asScreenCoord(position.x, position.y);
    tile.setPosition(tposition.x, tposition.y);
    tile.setTexture(game.getResources().getTexture("res/poop.png")); 
    game.getWindow().draw(tile);
}