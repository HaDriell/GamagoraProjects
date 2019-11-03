#pragma once

#include "maths.h"
#include "Framebuffer.h"

struct Camera
{
    //RT Transformation
    vec3 position;
    vec3 rotation;
    float fov;

    Framebuffer framebuffer;

    Camera() : position(vec3()), rotation(vec3()), fov(60), framebuffer(Framebuffer(256, 256)) {}

    float get_aspect_ratio();
    float get_focal_distance();
};