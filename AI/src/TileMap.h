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
    static const T NOTHING;

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
    T get(unsigned int x, unsigned int y) const
    {
        if (x >= width) return NOTHING;
        if (y >= height) return NOTHING;
        return tiles[x + y * width];
    }

    T get(vec2 p) const
    {
        return get(p.x, p.y);
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