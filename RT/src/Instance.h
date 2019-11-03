#pragma once

#include <random>

#include "maths.h"
#include "Material.h"

// pre-declaration for HitResult
struct Instance; 

struct HitResult
{
    bool            hit             = false;
    unsigned long   intersections   = 1;
    float           distance;
    vec3            hitPoint;
    vec3            normal;
    Instance*       instance;
};


struct Instance
{
    Transform transform;
    Material material;

    HitResult hit(const vec3& position, const vec3& direction);
    bool hitBoundingBox(const vec3& position, const vec3& direction);
    
    virtual ~Instance();
    virtual vec3 get_centroid() const = 0;
    virtual AABB get_bounding_box() const = 0;
    virtual bool intersectBoundingBox(const vec3& position, const vec3& direction) const = 0;
    virtual HitResult intersect(const vec3& position, const vec3& direction) = 0;
    virtual vec3 get_random_point_on_surface(std::default_random_engine& random, float bias) = 0;
};
