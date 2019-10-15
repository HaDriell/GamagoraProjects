#pragma once

#include <string>
#include <vector>

#include "maths.h"

#include "Shader.h"


class Mesh
{
private:
    unsigned int vertexArray;
    unsigned int indexBuffer;
    unsigned int elementCount;

    std::vector<unsigned int> vertexBuffers;

public:
    Mesh();
    ~Mesh();
    void destroy();

    void loadFromOBJ(const std::string& path);
    void loadFromMemory(std::vector<unsigned int>& indices, const std::vector<vec3>& vertices, const std::vector<vec3>& normals, const std::vector<vec2>& uvs);
    void draw(const Shader& shader);
};