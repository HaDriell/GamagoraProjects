#pragma once

#include "../math/math.h"
#include "Framebuffer.h"

struct Camera
{
    //RT Transformation
    vec3 position;
    vec3 rotation;
    float fov;

    Framebuffer framebuffer;

    Camera() : position(vec3()), rotation(vec3()), fov(60), framebuffer(Framebuffer(256, 256)) {}

//    void lookAt(vec3 target, vec3 up = vec3::UP);

    float get_aspect_ratio();
    float get_focal_distance();
};