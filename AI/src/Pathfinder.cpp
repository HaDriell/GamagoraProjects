#include "Pathfinder.h"

#include <limits>
#include <algorithm>


bool operator==(const DTile& a, const DTile& b)
{
    return a.position == b.position
        && a.cost == b.cost
        && a.weight == b.weight
        && a.parent == b.parent
        && a.visited == b.visited;
}

bool operator!=(const DTile& a, const DTile& b)
{
    return !(a == b);
}




/* Dijkstra map implementation */
std::vector<vec2> find_path_dijkstra(const WeightMap& weightmap, const vec2& start, const vec2& end)
{
    std::vector<vec2> path;

    DMap map = DMap(weightmap.get_width(), weightmap.get_height());
    for (unsigned int y = 0; y < map.get_height(); y++)
    {
        for (unsigned int x = 0; x < map.get_width(); x++)
        {
            DTile tile;
            tile.position = vec2(x, y);
            tile.weight   = weightmap.get(x, y);
            map.set(x, y, tile);
        }
    }

    std::vector<DTile> frontier;

    //Initialise Dijkstra
    DTile parent = map.get(start);

    if (parent == map.NOTHING) // Cannot find a path for a node that isn't found in the map in the first place
        return path;
    parent.visited = true;
    parent.cost = 0;
    frontier.push_back(parent);

    //Explore
    while(!frontier.empty() && parent.position != end)
    {
        //Find shortest walked path to explore
        auto found = std::min_element(frontier.begin(), frontier.end(), [] (DTile a, DTile b){
            return a.cost < b.cost;
        });
        parent = *found;
        frontier.erase(found);

        //Check neighborhood
        for (int y = -1; y <= 1; y++)
        {
            for (int x = -1; x <= 1; x++)
            {
                //skip self
                if (x == 0 && y == 0)
                    continue;

                //Compute neighbor position
                vec2 position = parent.position;
                position.x += x;
                position.y += y;

                DTile next = map.get(position);
                if (next != map.NOTHING) // protection against out of bound
                {
                    float cost = parent.cost + parent.weight * (x == 0 || y == 0 ? 1 : 1.41);
                    if (next.cost > cost)
                    {
                        next.cost = cost;
                        next.parent = parent.position;
                        if (!next.visited)
                        {
                            next.visited = true;
                            frontier.push_back(next);
                        }
                    }
                }
            }
        }
    }

    //Backtrack
    DTile origin = map.get(start);
    DTile track = map.get(end);
    while (track != map.NOTHING && track.position != origin.position)
    {
        path.push_back(track.position);
        track = map.get(track.parent);
    }
    std::reverse(path.begin(), path.end());

    return path;
}

/* A* map implementation */
/*
std::vector<vec2> find_path_a_star(const TerrainMap& terrain, const vec2& start, const vec2& end)
{
    std::vector<vec2> path;
    
    struct Tile
    {
        vec2 position;
        float weight;
        float g;// distance with start ( parent.g + parent.weight )
        float f;// f = g + h
        bool visited = false;
        Tile* parent = nullptr;
    };

    //build the Astar map (clean)
    TileMap<Tile> map(terrain.width, terrain.height);
    for (unsigned int y = 0; y < terrain.height; y++)
    {
        for (unsigned int x = 0; x < terrain.width; x++)
        {
            Tile* t = map.get(x, y);
            t->position = vec2{(float) x, (float) y};
            t->g = std::numeric_limits<float>::max();
            t->parent = nullptr;
            t->weight = (*terrain.get(x, y));
        }
    }

    std::vector<Tile*> openList;
    std::vector<Tile*> closedList;

    //Initialise openList
    Tile* startTile = map.get(start);
    startTile->g = 0;
    startTile->f = startTile->g + distanceSquared(start, end);
    startTile->parent = nullptr;
    openList.push_back(startTile);

    while (!openList.empty()) // might change
    {
        auto found = std::min_element(openList.begin(), openList.end(), [] (Tile* a, Tile* b){
            return a->f < b->f;
        });
        Tile* current = *found;

        //Target reached with a nearly optimal path !
        if (current->position == end)
            break;

        openList.erase(found);
        closedList.push_back(current);


        for (int y = -1; y <= 1; y++)
        {
            for (int x = -1; x <= 1; x++)
            {
                //skip self
                if (x == 0 && y == 0)
                    continue;
                
                vec2 position = current->position;
                position.x += x;
                position.y += y;
                Tile* tile = map.get(position);

                //Skip if tile is not found on the map (memory overflow protection)
                if (!tile)
                    continue;

                //Skip if tile is in closedList
                if (std::find(closedList.begin(), closedList.end(), tile) != closedList.end())
                    continue;
                
                //Compute cost to travel to tile
                float weight = (current->weight + tile->weight) / 2;
                float distance = (x == 0 || y == 0) ? 1 :  1.41; // assume unit squares. Diagonals are more expensive
                float cost = current->g + distance * weight;

                //Skip if Tile already has a better path
                if (cost > tile->g)
                    continue;
                
                //Update the tile to have current as a parent
                tile->g = cost;
                tile->f = cost + distanceSquared(tile->position, end);
                tile->parent = current;

                //Add tile to the openList if it's not in it already
                if (std::find(openList.begin(), openList.end(), tile) == openList.end())
                {
                    openList.push_back(tile);
                }
            }
        }
    }

    //Backtrack
    Tile* origin = map.get(start);
    Tile* track = map.get(end);
    while (track && track != origin)
    { 
        path.push_back(track->position);
        track = track->parent;
    }
    std::reverse(path.begin(), path.end());

    return path;
}

//*/