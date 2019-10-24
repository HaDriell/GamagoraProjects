#pragma once

#include <memory>
#include <string>
#include <vector>

#include "maths.h"

#include "Buffer.h"
#include "VertexArray.h"


const BufferLayout MESH_POSITION_LAYOUT = {{ VertexAttributeType::Float3, "vertex_position" }};
const BufferLayout MESH_NORMAL_LAYOUT   = {{ VertexAttributeType::Float3, "vertex_normal" }};
const BufferLayout MESH_COLOR_LAYOUT    = {{ VertexAttributeType::Float3, "vertex_color" }};
const BufferLayout MESH_UV_LAYOUT       = {{ VertexAttributeType::Float2, "vertex_uv" }};

class Mesh
{
private:
    std::shared_ptr<VertexArray> vertexArray;

    std::shared_ptr<VertexBuffer> positions;    //location 0
    std::shared_ptr<VertexBuffer> normals;      //location 1
    std::shared_ptr<VertexBuffer> colors;       //location 2
    std::shared_ptr<VertexBuffer> uvs;          //location 3

    std::shared_ptr<IndexBuffer> indices;

public:
    Mesh();
    ~Mesh();

    const std::shared_ptr<VertexArray> getVertexArray() const { return vertexArray; }
    const std::shared_ptr<IndexBuffer> getIndexBuffer() const { return indices; }

    void setIndices(const std::vector<unsigned int>& indices);
    void setIndices(const std::vector<unsigned short>& indices);
    void setPositions(const std::vector<vec3>& positions);
    void setNormals(const std::vector<vec3>& normals);
    void setColors(const std::vector<vec3>& colors);
    void setUVs(const std::vector<vec2>& uvs);
};