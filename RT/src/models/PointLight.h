#pragma once

#include <glm/glm.hpp>

struct PointLight
{
    glm::vec3 position;
    float intensity;
    glm::vec3 color;
};