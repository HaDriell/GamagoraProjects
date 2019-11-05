#pragma once


#include "../Common.h"


class Camera
{
private:
    mat4 projection;
    mutable mat4 view;

    vec3 position;
    vec3 rotation;
public:
    Camera(const mat4& projectionMatrix, const vec3& position, const vec3& rotation);
};