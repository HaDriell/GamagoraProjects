#pragma once

#include "Instance.h"

struct Sphere : public Instance
{
    virtual HitResult intersect(const vec3& position, const vec3& direction) override;
    virtual vec3 get_random_point_on_surface(float bias) override;
};