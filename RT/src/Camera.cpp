#include "Camera.h"
#include <math.h>

float Camera::get_aspect_ratio()
{
    return framebuffer.width / (float) framebuffer.height;
}

float Camera::get_focal_distance()
{
    return (framebuffer.width * 0.5f) / (std::tan(deg2rad * fov * 0.5f));
}