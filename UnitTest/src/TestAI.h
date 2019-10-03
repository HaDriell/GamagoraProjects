#include "TestUtil.h"
#include "Pathfinder.h"

test_dijkstra();
test_a_star();

void test_ai()
{
    test_dijkstra();
    test_a_star();
}

void test_dijkstra()
{
    TileMap<float> terrain(9, 9);
    for (int x = 0; x < terrain.width; x++)
    {
        for (int y = 0; y < terrain.height; y++)
        {
            terrain.set(x, y, 1.f); // walkable
        }
    }

    terrain.set(5, 0, 10000); // wall
    terrain.set(5, 1, 10000); // wall
    terrain.set(5, 2, 10000); // wall
    terrain.set(5, 3, 10000); // wall

    std::vector<Point> path = find_path_dijkstra(terrain, Point{0, 0}, Point{8, 0});
    assert(std::find(path.begin(), path.end(), Point{5, 4}) != path.end());
}

void test_a_star()
{
    TileMap<float> terrain(9, 9);
    for (int x = 0; x < terrain.width; x++)
    {
        for (int y = 0; y < terrain.height; y++)
        {
            terrain.set(x, y, 1.f); // walkable
        }
    }

    terrain.set(5, 0, 1e10); // wall
    terrain.set(5, 1, 1e10); // wall
    terrain.set(5, 2, 1e10); // wall
    terrain.set(5, 3, 1e10); // wall

    std::vector<Point> path = find_path_a_star(terrain, Point{0, 0}, Point{8, 0});
    assert(std::find(path.begin(), path.end(), Point{5, 4}) != path.end());
}
