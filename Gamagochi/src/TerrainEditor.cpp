#include "TerrainEditor.h"
#include "Fruit.h"

void TerrainEditor::event(Game& game, const sf::Event& event)
{
    //Tool Selection
    if (event.type == sf::Event::KeyPressed)
    {
        //Selection mode
        if (event.key.code == sf::Keyboard::Num1) mode = 1;
        if (event.key.code == sf::Keyboard::Num2) mode = 2;
        if (event.key.code == sf::Keyboard::Num3) mode = 3;
        if (event.key.code == sf::Keyboard::Num4) mode = 4;
        if (event.key.code == sf::Keyboard::Num5) mode = 5;
        if (event.key.code == sf::Keyboard::Num6) mode = 6;
        if (event.key.code == sf::Keyboard::Num7) mode = 7;
        if (event.key.code == sf::Keyboard::Num8) mode = 8;
        if (event.key.code == sf::Keyboard::Num9) mode = 9;
        if (event.key.code == sf::Keyboard::Num0) mode = 0;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        //Fruit dropping tool (event driven because we don't want to flood the world of fruits)
        if (mode == 4)
        {
            game.getEntities().push_back(new Fruit(game.getMouseTile()));
        }
    }
}

void TerrainEditor::udpate(Game& game)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        switch (mode)
        {
            //Terrain edition tools (polling mode because we want easy editing)
            case 1: game.getTerrain().set(game.getMouseTile(), TerrainType::Grass); break;
            case 2: game.getTerrain().set(game.getMouseTile(), TerrainType::Sand); break;
            case 3: game.getTerrain().set(game.getMouseTile(), TerrainType::Rock); break;
        }
    }
}

void TerrainEditor::render(Game& game)
{
    sf::Sprite tile;
    vec2 position = game.getMouseTile();
    position = game.asScreenCoord(position.x, position.y);
    tile.setPosition(position.x, position.y);
    tile.setScale(2, 2);
    switch (mode)
    {
        case 1:
            tile.setTexture(game.getResources().getTexture("res/grass.png"));
        break;

        case 2:
            tile.setTexture(game.getResources().getTexture("res/sand.png"));
        break;
        
        case 3:
            tile.setTexture(game.getResources().getTexture("res/rock.png"));
        break;    

        case 4:
            tile.setTexture(game.getResources().getTexture("res/apple.png"));
        break;    
    }
    float effect = 100 + 155 * std::abs(std::sin(game.getUpTime() * 20));
    tile.setColor(sf::Color(effect, effect, effect));

    game.getWindow().draw(tile);
}
