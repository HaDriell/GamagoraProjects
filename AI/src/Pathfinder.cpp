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

bool operator==(const ATile& a, const ATile& b)
{
    return a.position == b.position
        && a.g == b.g
        && a.f == b.f
        && a.weight == b.weight
        && a.parent == b.parent
        && a.visited == b.visited;
}


bool operator!=(const ATile& a, const ATile& b)
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
            weightmap.get(x, y, tile.weight);
            map.set(x, y, tile);
        }
    }

    std::vector<DTile> frontier;

    //Initialise Dijkstra
    DTile parent;
    if(!map.get(start, parent)) // no path found
        return path;

    parent.visited = true;
    parent.cost = 0;
    frontier.push_back(parent);

    //Explore
    while(!frontier.empty() && parent.position != end)
    {
        //Find shortest walked path to explore
        auto found = std::min_element(frontier.begin(), frontier.end(), [] (const DTile& a, const DTile& b){
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

                DTile next;
                if (map.get(position, next))
                {
                    float cost = parent.cost + parent.weight * (x == 0 || y == 0 ? 1 : 1.41);
                    if (next.cost > cost)
                    {
                        next.cost = cost;
                        next.parent = parent.position;
                        map.set(position, next);
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
    DTile origin;
    DTile track;
    map.get(start, origin);
    map.get(end, track);
    while (track.position != origin.position)
    {
        path.push_back(track.position);
        if (!map.get(track.parent, track))
            break;
    }
    std::reverse(path.begin(), path.end());

    return path;
}

/* A* map implementation */

std::vector<vec2> find_path_a_star(const WeightMap& terrain, const vec2& start, const vec2& end)
{
    std::vector<vec2> path;

    //build the Astar map (clean)
    AMap map = AMap(terrain.get_width(), terrain.get_height());
    for (unsigned int y = 0; y < map.get_height(); y++)
    {
        for (unsigned int x = 0; x < map.get_width(); x++)
        {
            ATile tile;
            if (map.get(x, y, tile))
            {
                tile.position = vec2(x, y);
                tile.g = std::numeric_limits<float>::max();
                terrain.get(x, y, tile.weight);
                map.set(x, y, tile);
            }
        }
    }

    std::vector<ATile> openList;
    std::vector<ATile> closedList;

    //Initialise openList
    ATile startTile;
    map.get(start, startTile);
    startTile.g = 0;
    startTile.f = startTile.g + vec2::distance2(start, end);
    openList.push_back(startTile);

    while (!openList.empty()) // might change
    {
        auto found = std::min_element(openList.begin(), openList.end(), [] (const ATile& a, const ATile& b){
            return a.f < b.f;
        });
        ATile current = *found;

        //Target reached with a heuristically optimal path !
        if (current.position == end)
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
                
                vec2 position = current.position;
                position.x += x;
                position.y += y;

                //Skip if tile is not found on the map (memory overflow protection)
                ATile tile;
                if (!map.get(position, tile))
                    continue;
                else
                    std::cout << "Unable to find " << position << std::endl;

                //Skip if tile is in closedList
                if (std::find_if(closedList.begin(), closedList.end(), [&] (const ATile& t) { return t.position == tile.position; }) != closedList.end())
                    continue;
                
                //Compute cost to travel to tile
                float weight = (current.weight + tile.weight) / 2;
                float distance = (x == 0 || y == 0) ? 1 :  1.41; // assume unit squares. Diagonals are more expensive
                float cost = current.g + distance * weight;

                //Skip if Tile already has a better path
                if (tile.g < cost)
                    continue;
                
                //Update the tile to have current as a parent
                tile.g = cost;
                tile.f = cost + vec2::distance2(tile.position, end);
                tile.parent = current.position;
                map.set(x, y, tile);

                //Add tile to the openList if it's not in it already
                if (std::find_if(openList.begin(), openList.end(), [&](const ATile& t) { return t.position == tile.position; }) == openList.end())
                    openList.push_back(tile);
            }
        }
    }

    //Backtrack
    ATile origin;
    ATile track;
    map.get(start, origin);
    map.get(end, track);
    while (track.position != origin.position)
    {
        std::cout << "Path " << track.position << std::endl;
        path.push_back(track.position);
        if (!map.get(track.parent, track))
            break;
    }
    std::reverse(path.begin(), path.end());

    return path;
}