#include "Gamagochi.h"
#include "Poop.h"




void Gamagochi::event(Game& game, const sf::Event& event)
{
}

void Gamagochi::udpate(Game& game)
{
    switch (goal)
    {
        case Idle:
        {
            if (hungerCooldown.elapsed() > 3)
                goal = LookingForFood;
        }
        break;

        case LookingForFood:
        {
            std::vector<Entity*> fruits = game.getFruits();

            //No Food found ! Keep looking for it next frame
            if (fruits.empty()) 
                break;

            //Food found ! Take the nearest one
            target = fruits[0]->position;
            for (Entity* e : game.getFruits())
            {
                if (vec2::distance2(position, target) > vec2::distance2(position, e->position))
                    target = e->position;
            }
            // Now it's time to move towards the food
            goal = ChasingFood;
        }
        break;

        case ChasingFood:
        {
            if (vec2::distance2(position, target) < 1)
            {
                goal = EatingFood;
                break;
            }

            if (movingCooldown.elapsed() > 0.2)
            {
                movingCooldown.reset(); // reset cooldown

                //Build WeightMap
                Terrain terrain = game.getTerrain();
                WeightMap map = WeightMap(terrain.get_width(), terrain.get_height());
                for(int x = 0; x < terrain.get_width(); x++)
                {
                    for(int y = 0; y < terrain.get_height(); y++)
                    {
                        float weight = 0;
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

                //Disgustang ! Add some weight to the neighborhood
                for (Entity* poop : game.getPoops())
                {
                    //3 by 3 square
                    for (int x = -1; x <= 1; x++)
                    {
                        for (int y = -1; y <= 1; y++)
                        {
                            float weight;
                            vec2 position = poop->position;
                            position.x += x;
                            position.y += y;
                            map.get(position, weight);
                            map.set(position, weight + 250);
                        }
                    }
                }

                //Find the way & move
                std::vector<vec2> path = find_path_dijkstra(map, position, target);
                path = find_path_a_star(map, position, target);
                if (!path.empty())
                {
                    position = path[0]; // teleport !
                }
            }
        }
        break;

        case EatingFood:
        {
            //Eat all the fruits at position
            for (Entity* fruit : game.getFruits())
            {
                if (vec2::distance2(fruit->position, position) < 1)
                {
                    fruit->destroy();
                }
            }

            //Eating spawns a Poop
            game.getEntities().push_back(new Poop(position));

            //Reset hunger & go Idle for a while
            hungerCooldown.reset();
            goal = Idle;
            break;
        }
        break;
    }
}

void Gamagochi::render(Game& game)
{
    sf::Sprite tile = sf::Sprite();
    tile.setScale(2, 2);
    vec2 tposition = game.asScreenCoord(position.x, position.y);
    tile.setPosition(tposition.x, tposition.y);
    tile.setTexture(game.getResources().getTexture("res/gamagochi.png")); 
    tile.setColor(sf::Color::White);
    if (goal == ChasingFood)
        tile.setColor(sf::Color::Red);
    if (goal == LookingForFood)
        tile.setColor(sf::Color::Magenta);
    game.getWindow().draw(tile);
}