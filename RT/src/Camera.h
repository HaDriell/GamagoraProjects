#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


struct Camera
{
    glm::vec3 position;
    glm::quat orientation;
    float fov;
};