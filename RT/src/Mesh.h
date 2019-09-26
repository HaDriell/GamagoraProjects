#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

struct Mesh
{
    std::vector<glm::vec3> positions; // vertex position
    std::vector<glm::vec3> colors;    // vertex color
    std::vector<glm::vec3> normals;   // vertex normal
    std::vector<glm::vec2> uvs;       // vertex uv coord
    std::vector<unsigned int> indices;// index 

    void load_obj(const std::string& path);
    void load_off(const std::string& path);
};