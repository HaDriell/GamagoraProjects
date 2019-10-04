#pragma once

#include "vec2.h"
#include <vector>
#include <iostream>

#include "TileMap.h"


//Dijkstra Tile
struct DTile
{
    vec2 position;
    float weight = std::numeric_limits<float>::max();
    float cost = std::numeric_limits<float>::max();
    bool visited = false;
    vec2 parent;
};

bool operator==(const DTile& a, const DTile& b);
bool operator!=(const DTile& a, const DTile& b);


using WeightMap = TileMap<float>;
using DMap      = TileMap<DTile>;

std::vector<vec2> find_path_dijkstra(const WeightMap& terrain, const vec2& start, const vec2& end);
std::vector<vec2> find_path_a_star(const WeightMap& terrain, const vec2& start, const vec2& end);
