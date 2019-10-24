#pragma once

#include "maths.h"

class Camera
{
private:
    mat4 projectionMatrix;
    mat4 viewMatrix;

public:
    Camera(const mat4& projectionMatrix);
};
