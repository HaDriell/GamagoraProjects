#include "PF.h"

#include <SFML/Window.hpp>


PF::PF() : Entity(GAMAGOCHI) {}

PF::~PF() {}

void PF::computePath(const Game& game)
{
    //Build Map from the Terrain
    Terrain terrain = game.getTerrain();
    WeightMap map(terrain.get_width(), terrain.get_height());
    for (int x = 0; x < terrain.get_width(); x++)
    {
        for (int y = 0; y < terrain.get_height(); y++)
        {
            TerrainType type = terrain.get(x, y);
            if (type == TerrainType::Grass)
                map.set(x, y, 10);
            if (type == TerrainType::Sand)
                map.set(x, y, 14);
            if (type == TerrainType::Rock)
                map.set(x, y, 1000000);
        }
    }

    //Update the Path
    path = find_path_dijkstra(map, start, end);
}


void PF::event(const Game& game, const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            start = game.asTile(event.mouseButton.x, event.mouseButton.y);
            std::cout << "Start " << start << std::endl;
            std::cout << "End   " << end   << std::endl;
            computePath(game);
        }

        if (event.mouseButton.button == sf::Mouse::Right)
        {
            end = game.asTile(event.mouseButton.x, event.mouseButton.y);
            std::cout << "Start " << start << std::endl;
            std::cout << "End   " << end   << std::endl;
            computePath(game);
        }
    }
}

void PF::udpate(const Game& game)
{
    //no update
}

void PF::render(const Game& game)
{
    sf::RectangleShape tile;

    //Render Path
    tile.setSize({20, 20});
    tile.setFillColor(sf::Color::Blue);
    tile.setOutlineColor(sf::Color::Black);
    for (vec2 p : path)
    {
        vec2 tp = game.asTile(p.x, p.y);
        tile.setPosition(tp.x, tp.y);
        game.getWindow().draw(tile);
    }

    //Render Start
    tile.setFillColor(sf::Color::Cyan);
    vec2 tstart = game.asTile(start.x, start.y);
    tile.setPosition(tstart.x, tstart.y);
    game.getWindow().draw(tile);
    
    //Render End
    tile.setFillColor(sf::Color::Cyan);
    vec2 tend = game.asTile(end.x, end.y);
    tile.setPosition(tend.x, tend.y);
    game.getWindow().draw(tile);
}
