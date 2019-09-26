#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform 
{
    glm::vec3 translation = glm::vec3();
    glm::vec3 rotation = glm::vec3();
    glm::vec3 scale = glm::vec3{1.f, 1.f, 1.f};
};

glm::vec3 operator*(const Transform& transform, const glm::vec3& v);