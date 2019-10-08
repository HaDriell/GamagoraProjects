#pragma once

#include <vector>

#include "maths.h"

struct Vertex
{
    vec3 position;
    vec3 color;
};

struct Mesh
{
    std::vector<Vertex> vertices;

    void load_off_file(const std::string& path);
    void load_obj_file(const std::string& path);
};
