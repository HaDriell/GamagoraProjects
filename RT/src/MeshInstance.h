#pragma once


#include <vector>
#include <string>

#include "Instance.h"
#include "Mesh.h"
#include "maths.h"


//BVH Accerelation Structure for high poly mesh instances
struct MeshBVH
{
    virtual MeshBVH() = 0;
    virtual HitResult intersect(const vec3& position, const vec3& direction) = 0;
};

struct MeshBVHNode : public MeshBVH
{
    MeshBVH* left;
    MeshBVH* right;
    AABB aabb;

    MeshBVHNode(const std::vector<Vertex> vertices);

    virtual ~MeshBVHNode();
    virtual HitResult intersect(const vec3& position, const vec3& direction) override;
};

struct MeshBVHLeaf : public MeshBVH
{
    std::vector<unsigned int> triangles;

    virtual HitResult intersect(const vec3& position, const3& direction) override;
};

struct MeshInstance : public Instance
{
    Mesh mesh;

    virtual ~MeshInstance();

    virtual HitResult intersect(const vec3& position, const vec3& direction);
    virtual vec3 get_random_point_on_surface(std::default_random_engine& random, float bias);
};