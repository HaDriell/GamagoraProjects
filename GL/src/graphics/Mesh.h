#pragma once

#include <vector>

#include "maths.h"


class Mesh
{
public:
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec3> uvs;
    std::vector<unsigned int> indices;
};