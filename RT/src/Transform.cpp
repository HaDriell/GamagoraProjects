#include "Transform.h"

#include <glm/gtx/euler_angles.hpp>

//SRT order
glm::vec3 operator*(const Transform& transform, const glm::vec3& v)
{
    glm::mat3 rotation = glm::orientate3(transform.rotation);
    return transform.translation + (rotation * (transform.scale * v));
}