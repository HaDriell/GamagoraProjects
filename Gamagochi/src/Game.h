#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "vec2.h"

#include "Resources.h"
#include "Pathfinder.h"
#include "Timer.h"

/*
    Class that acts as the engine for the Gamagochi mini project
*/


struct TerrainType
{
    unsigned int id;

    static const TerrainType Grass;
    static const TerrainType Sand;
    static const TerrainType Rock;
};
bool operator==(const TerrainType& a, const TerrainType& b);
bool operator!=(const TerrainType& a, const TerrainType& b);

using Terrain = TileMap<TerrainType>;



class Entity;

class Game
{
private:
//SFML data
    sf::VideoMode           videoMode;
    sf::RenderWindow*       window;


//Game data
    Timer                   frameTimer;
    Timer                   upTimer;
    float                   deltaTime;
    Terrain*                terrain;
    Resources               resources;
    std::vector<Entity*>    entities;

private:
    void renderTerrain();

public:
    vec2 getMousePosition() const;
    vec2 getMouseTile() const;
    vec2 asTile(int x, int y) const;
    vec2 asScreenCoord(int x, int y) const;
    float getDeltaTime() const;
    float getUpTime() const;

    Terrain& getTerrain() const;
    Resources& getResources();
    sf::RenderWindow& getWindow() const;
    std::vector<Entity*>& getEntities();
    std::vector<Entity*> getFruits() const;
    std::vector<Entity*> getPoops() const;

    void spawnPoop(int x, int y) const;



public:
    Game();
    ~Game();
    void poll();
    void update();
    void render();
    bool isRunning();
};