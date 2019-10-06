#include "Game.h"
#include "Entity.h"
#include "Resources.h"

#include <SFML/Window.hpp>
#include <thread>
#include <chrono>
#include <algorithm>

const TerrainType TerrainType::Grass    = TerrainType{1};
const TerrainType TerrainType::Sand     = TerrainType{2};
const TerrainType TerrainType::Rock     = TerrainType{3};

bool operator==(const TerrainType& a, const TerrainType& b) {
    return a.id == b.id; 
}

bool operator!=(const TerrainType& a, const TerrainType& b) {
    return !(a == b); 
}

Game::Game()
{
    videoMode = sf::VideoMode(800, 600);
    window = new sf::RenderWindow(videoMode, "Gamagochi");

    //Create a basic terrain
    terrain = new Terrain(40, 30);
    for (int y = 0; y < terrain->get_height(); y++)
    {
        for (int x = 0; x < terrain->get_width(); x++)
        {
            //Make the inside walkable
            terrain->set(x, y, TerrainType::Grass);
            //Wall the oustide of the map
            if (x == 0 || x == terrain->get_width() - 1 || y == 0 || y == terrain->get_height() - 1) 
                terrain->set(x, y, TerrainType::Rock);
        }
    }
}

Game::~Game()
{
    delete window;
}

// Functions

void Game::poll()
{
    sf::Event event;
    while(window->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window->close();
        break;
        
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape)
                window->close();
        break;
        }

        for (Entity* e : entities)
        {
            e->event(*this, event);
        }
    }
}

void Game::update()
{
    deltaTime = frameTimer.elapsed();
    frameTimer.reset();

    //Update all entities
    for(Entity* e : entities)
    {
        e->udpate(*this);
    }

    //Eventually destroyed entities are pushed back in the array
    std::sort(entities.begin(), entities.end(), [](Entity* a,  Entity* b){
        return a->isDestroyed() < b->isDestroyed();
    });
    auto it = std::find_if(entities.begin(), entities.end(), [] (Entity* e) {
        return e->isDestroyed();
    });

    while (it != entities.end())
    {
        delete entities.back();
        entities.pop_back();
    }

    //Tick rate fixing (60Hz)
    int milliseconds = (int) (frameTimer.elapsed() - 16);
    if (milliseconds > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
}

void Game::renderTerrain()
{
    sf::Sprite tile = sf::Sprite();
    tile.setScale(2, 2);
    for (int y = 0; y < terrain->get_height(); y++)
    {
        for (int x = 0; x < terrain->get_width(); x++)
        {
            TerrainType type;
            if (terrain->get(x, y, type))
            {
                if (type == TerrainType::Grass)
                    tile.setTexture(resources.getTexture("res/grass.png")); 
                if (type == TerrainType::Sand)
                    tile.setTexture(resources.getTexture("res/sand.png")); 
                if (type == TerrainType::Rock)
                    tile.setTexture(resources.getTexture("res/rock.png"));
                tile.setPosition(x * 20, y * 20);
                window->draw(tile);
            }
        }
    }
}

void Game::render()
{
    //Clear buffer
    window->clear(sf::Color::Black);

    //Render terrain
    renderTerrain();

    //Render all entities
    for (Entity* e : entities)
    {
        e->render(*this);
    }

    //Swap buffers
    window->display();
}

bool Game::isRunning()
{
    return window && window->isOpen();
}

vec2 Game::getMousePosition() const
{
    sf::Vector2 mouse = sf::Mouse::getPosition(*window);
    return vec2(mouse.x, mouse.y);
}

vec2 Game::getMouseTile() const
{
    sf::Vector2 mouse = sf::Mouse::getPosition(*window);
    return asTile(mouse.x, mouse.y);
}

vec2 Game::asTile(int x, int y) const
{
    return vec2((float) (x / 20), (float) (y / 20));
}

vec2 Game::asScreenCoord(int x, int y) const
{
    return vec2(x * 20.f, y * 20.f);
}

Terrain& Game::getTerrain() const
{
    return *terrain;
}

sf::RenderWindow& Game::getWindow() const
{
    return *window;
}

float Game::getDeltaTime() const
{
    return deltaTime;
}


float Game::getUpTime() const
{
    return upTimer.elapsed();
}

Resources& Game::getResources()
{
    return resources;
}
std::vector<Entity*>& Game::getEntities()
{
    return entities;
}

std::vector<Entity*> Game::getFruits() const
{
    std::vector<Entity*> found;
    for (Entity* e : entities)
    {
        if (e->type == FRUIT)
            found.push_back(e);
    }
    return found;
}

std::vector<Entity*> Game::getPoops() const
{
    std::vector<Entity*> found;
    for (Entity* e : entities)
    {
        if (e->type == POOP)
            found.push_back(e);
    }
    return found;
}