#pragma once

#include <vector>
#include <iostream>

struct Point
{
    float x, y;
};

float distanceSquared(const Point& a, const Point& b);

bool operator==(const Point& a, const Point& b);
bool operator!=(const Point& a, const Point& b);
std::ostream& operator<<(std::ostream& os, const Point& p);

template<typename T>
class TileMap
{
public:
    const unsigned int width;
    const unsigned int height;
private:
    T* data;

public:
    TileMap(unsigned int width, unsigned int height) : width(width), height(height)
    {
        this->data = new T[width * height];
    }

    ~TileMap()
    {
        delete[] data;
    }

    T* get(unsigned int x, unsigned int y) const
    {
        if (x >= width) return nullptr;
        if (y >= height) return nullptr;
        return &data[x + y * width];
    }

    void set(unsigned int x, unsigned int y, T value)
    {
        if (x >= width) return;
        if (y >= height) return;
        data[x + y * width] = value;
    }

    T* get(Point p) const
    {
        return get(p.x, p.y);
    }
};

using TerrainMap = TileMap<float>;

std::vector<Point> find_path_dijkstra(const TerrainMap& terrain, const Point& start, const Point& end);
std::vector<Point> find_path_a_star(const TerrainMap& terrain, const Point& start, const Point& end);
