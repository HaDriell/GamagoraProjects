#pragma once

#include <glm/glm.hpp>
#include "Sphere.h"

struct CubeLight
{
    glm::vec3 position;
    float size;
    float intensity;
    glm::vec3 color;
};