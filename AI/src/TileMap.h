#pragma once

#include <functional>
#include <vector>
#include "vec2.h"

template<typename T>
class TileMap
{
private:
    unsigned int width;
    unsigned int height;
    std::vector<T> tiles;

public:

    TileMap(unsigned int width = 1, unsigned int height = 1)
    {
        resize(width, height);
    }

    unsigned int get_width() const
    {
        return width;
    }

    unsigned int get_height() const
    {
        return height;
    }

    void resize(unsigned int width, unsigned int height)
    {
        this->width = width;
        this->height = height;
        tiles.resize(width * height);
    }

    //TODO : have to use a bool to filter the get errors. sad but it's way too late to continue right now
    bool get(unsigned int x, unsigned int y, T& out) const
    {
        if (x >= width) return false;
        if (y >= height) return false;
        out = tiles[x + y * width];
        return true;
    }

    bool get(vec2 p, T& out) const
    {
        return get(p.x, p.y, out);
    }

    void set(unsigned int x, unsigned int y, const T& value)
    {
        if (x >= width) return;
        if (y >= height) return;
        tiles[x + y * width] = value;
    }
    
    void set(vec2 p, const T& value)
    {
        set(p.x, p.y, value);
    }

};