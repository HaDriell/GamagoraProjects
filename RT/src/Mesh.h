#pragma once


#include <vector>
#include <string>

#include "Instance.h"
#include "maths.h"

struct Vertex 
{
    vec3 position;
    vec3 color;
};

struct Mesh : public Instance
{
    std::vector<Vertex> vertices;

    virtual ~Mesh();

    virtual HitResult intersect(const vec3& position, const vec3& direction);
    virtual vec3 get_random_point_on_surface(std::default_random_engine& random, float bias);

    void debug() const;
    void load_off_file(const std::string& path);
    void load_obj_file(const std::string& path);
};