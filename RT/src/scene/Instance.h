#pragma once

#include "../math/math.h"
#include "Material.h"

// pre-declaration for HitResult
struct Instance; 

struct HitResult
{
    bool hit;
    float distance;
    vec3 hitPoint;
    vec3 normal;
    Instance* instance;

    HitResult() : hit(false) {}
};


struct Instance
{
    transform transform;
    Material material;

    HitResult hit(const vec3& position, const vec3& direction);
    
    virtual HitResult intersect(const vec3& position, const vec3& direction) = 0;
    virtual vec3 get_random_point_on_surface(float bias) = 0;
};
