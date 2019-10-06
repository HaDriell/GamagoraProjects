#include "PF.h"

#include <SFML/Window.hpp>


PF::PF() : Entity(SYSTEM), astar(false) {}

PF::~PF() {}


void PF::use_astar()
{
    astar = true;
}

void PF::use_dijkstra()
{
    astar = false;
}

void PF::computePath(const Game& game)
{
    //Build Map from the Terrain
    Terrain terrain = game.getTerrain();
    WeightMap map(terrain.get_width(), terrain.get_height());
    for (int x = 0; x < terrain.get_width(); x++)
    {
        for (int y = 0; y < terrain.get_height(); y++)
        {
            TerrainType type;
            if (terrain.get(x, y, type))
            {
                if (type == TerrainType::Grass)
                    map.set(x, y, 10);
                if (type == TerrainType::Sand)
                    map.set(x, y, 14);
                if (type == TerrainType::Rock)
                    map.set(x, y, 1000000);
            }
        }
    }

    //Update the Path
    if (astar)  path = find_path_a_star(map, start, end);
    else        path = find_path_dijkstra(map, start, end);
}


void PF::event(Game& game, const sf::Event& event)
{
    if (event.type == sf::Event::MouseMoved)
    {
        hoveredTile = game.asTile(event.mouseMove.x, event.mouseMove.y);
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::A)
            astar = !astar;
        
        if (event.key.code == sf::Keyboard::Z)
        {
            start = hoveredTile;
            computePath(game);
        }

        if (event.key.code == sf::Keyboard::E)
        {
            end = hoveredTile;
            computePath(game);
        }
    }

}

void PF::udpate(Game& game)
{
    //no update
}

void PF::render(Game& game)
{
    sf::RectangleShape tile;

    //Render Path
    tile.setSize({20, 20});
    tile.setFillColor(sf::Color::Blue);
    tile.setOutlineColor(sf::Color::Black);
    for (vec2 p : path)
    {
        vec2 tp = game.asScreenCoord(p.x, p.y);
        tile.setPosition(tp.x, tp.y);
        game.getWindow().draw(tile);
    }

    //Render Start
    tile.setFillColor(sf::Color::Cyan);
    vec2 tstart = game.asScreenCoord(start.x, start.y);
    tile.setPosition(tstart.x, tstart.y);
    game.getWindow().draw(tile);
    
    //Render End
    tile.setFillColor(sf::Color::Red);
    vec2 tend = game.asScreenCoord(end.x, end.y);
    tile.setPosition(tend.x, tend.y);
    game.getWindow().draw(tile);
}