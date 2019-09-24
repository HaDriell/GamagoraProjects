#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

struct Face
{
    unsigned int  v[3];
    unsigned int vn[3];
    unsigned int vt[3];
};

struct Mesh
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;         //unsupported for now

    std::vector<Face> faces;
};

void load_mesh_obj(const std::string& filename, std::vector<Mesh*>& meshes);