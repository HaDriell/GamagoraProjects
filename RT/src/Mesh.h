#pragma once


#include <vector>
#include <string>

#include "Instance.h"
#include "maths.h"

struct Mesh : public Instance
{
    std::vector<vec3> positions;
    std::vector<vec3> colors;
    std::vector<vec3> normals;
    std::vector<vec2> uvs;
    std::vector<unsigned int> indices;

    virtual ~Mesh();

    virtual HitResult intersect(const vec3& position, const vec3& direction);
    virtual vec3 get_random_point_on_surface(std::default_random_engine& random, float bias);

    vec3 compute_centroid();

    void normalize();
    void load_off_file(const std::string& path);
    void load_obj_file(const std::string& path);
};