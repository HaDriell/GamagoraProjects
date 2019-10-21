#pragma once

#include "Instance.h"

struct SphereInstance : public Instance
{
    virtual ~SphereInstance();
    virtual vec3 get_centroid() const override;
    virtual AABB get_bounding_box() const override;
    virtual bool intersectBoundingBox(const vec3& position, const vec3& direction) const override;
    virtual HitResult intersect(const vec3& position, const vec3& direction) override;
    virtual vec3 get_random_point_on_surface(std::default_random_engine& random, float bias) override;
};