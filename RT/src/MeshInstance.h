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

    virtual vec3 get_centroid() const;
    virtual AABB get_bounding_box() const;
    virtual bool intersectBoundingBox(const vec3& position, const vec3& direction) const override;
    virtual HitResult intersect(const vec3& position, const vec3& direction) override;
    virtual vec3 get_random_point_on_surface(std::default_random_engine& random, float bias) override;
};