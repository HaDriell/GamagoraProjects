#include "Pathfinder.h"

#include <limits>
#include <algorithm>

float distanceSquared(const Point& a, const Point& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx*dx + dy*dy;
}

bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x && a.y == b.y;
}

bool operator!=(const Point& a, const Point& b)
{
    return !(a == b);
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

/* Dijkstra map implementation */
std::vector<Point> find_path_dijkstra(const TerrainMap& terrain, const Point& start, const Point& end)
{
    std::vector<Point> path;

    struct Tile
    {
        Point position;
        float weight = std::numeric_limits<float>::max();
        float cost = std::numeric_limits<float>::max();
        bool visited = false;
        Tile* parent = nullptr;
    };

    TileMap<Tile> dmap(terrain.width, terrain.height);
    for (unsigned int y = 0; y < terrain.height; y++)
    {
        for (unsigned int x = 0; x < terrain.width; x++)
        {
            Tile* t = dmap.get(x, y);
            t->position = Point{(float) x, (float) y};
            t->weight = *( terrain.get(x, y) );
        }
    }

    std::vector<Tile*> frontier;

    //Initialise Dijkstra
    Tile* parent = dmap.get(start);
    parent->visited = true;
    parent->cost = 0;
    frontier.push_back(parent);

    //Explore
    while(!frontier.empty() && parent->position != end)
    {
        //Find shortest walked path to explore
        auto found = std::min_element(frontier.begin(), frontier.end(), [] (Tile* a, Tile* b){
            return a->cost < b->cost;
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
                Point position = parent->position;
                position.x += x;
                position.y += y;

                Tile* next = dmap.get(position);
                if (next) // protection against out of bound coords
                {
                    float cost = parent->cost + parent->weight * (x == 0 || y == 0 ? 1 : 1.41);
                    if (next->cost > cost)
                    {
                        next->cost = cost;
                        next->parent = parent;
                        if (!next->visited)
                        {
                            next->visited = true;
                            frontier.push_back(next);
                        }
                    }
                }
            }
        }
    }

    //Backtrack
    Tile* origin = dmap.get(start);
    Tile* track = dmap.get(end);
    while (track && track != origin)
    {
        path.push_back(track->position);
        track = track->parent;
    }
    std::reverse(path.begin(), path.end());

    return path;
}

/* A* map implementation */
std::vector<Point> find_path_a_star(const TerrainMap& terrain, const Point& start, const Point& end)
{
    std::vector<Point> path;
    
    struct Tile
    {
        Point position;
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
            t->position = Point{(float) x, (float) y};
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
                
                Point position = current->position;
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