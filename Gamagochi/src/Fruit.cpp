#include "Fruit.h"




void Fruit::event(Game& game, const sf::Event& event)
{
}

void Fruit::udpate(Game& game)
{
}

void Fruit::render(Game& game)
{
    sf::Sprite tile = sf::Sprite();
    tile.setScale(2, 2);
    vec2 tposition = game.asScreenCoord(position.x, position.y);
    tile.setPosition(tposition.x, tposition.y);
    tile.setTexture(game.getResources().getTexture("res/apple.png")); 
    game.getWindow().draw(tile);
}