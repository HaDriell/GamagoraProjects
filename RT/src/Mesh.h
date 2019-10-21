#pragma once

#include <vector>

#include "maths.h"

class MeshBVH
{
private:
    AABB box;
    MeshBVH* left;
    MeshBVH* right;
    std::vector<Triangle*> triangles;
    int level;
    
public:
    MeshBVH(const std::vector<Triangle*>& triangles, int maxTriangles = 4, int maxLevels = 15);
    ~MeshBVH();

    AABB get_bounding_box() const;
    bool intersect(const vec3& position, const vec3& direction, float& distance, vec3& hitPoint, vec3& normal) const;
};

struct Mesh
{
private:
    std::vector<Triangle*> triangles;
    MeshBVH* bvh = nullptr;

public:
    ~Mesh();
    void clear();
    void set(const std::vector<vec3> vertices);
    void load_off_file(const std::string& path);
    void load_obj_file(const std::string& path);

    vec3 get_centroid() const;
    AABB get_bounding_box() const;
    bool intersect(const vec3& position, const vec3& direction, float& distance, vec3& hitPoint, vec3& normal) const;
};
