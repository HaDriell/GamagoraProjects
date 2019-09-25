#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
    unsigned int v0 = 0,  v1 = 0,  v2 = 0;
    unsigned int vn0 = 0, vn1 = 0, vn2 = 0;
    unsigned int vt0 = 0, vt1 = 0, vt2 = 0;
};

struct Mesh
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<Vertex> vertices;

    void load_obj(const std::string& path);
};