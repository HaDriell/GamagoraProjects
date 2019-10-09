#pragma once


#include <vector>
#include <string>

#include "Instance.h"
#include "Mesh.h"
#include "maths.h"

struct MeshInstance : public Instance
{
    Mesh mesh;

    virtual ~MeshInstance();

    virtual HitResult intersect(const vec3& position, const vec3& direction);
    virtual vec3 get_random_point_on_surface(std::default_random_engine& random, float bias);
};