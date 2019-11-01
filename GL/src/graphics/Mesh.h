#pragma once


#include "../Common.h"

#include "Buffer.h"
#include "VertexArray.h"


struct MeshVertex
{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;
};

const BufferLayout MeshVertexLayout = {
    { VertexAttributeType::Float3, "Position" },
    { VertexAttributeType::Float3, "Normal"   },
    { VertexAttributeType::Float3, "Color"    },
    { VertexAttributeType::Float2, "UV"       },
};

class Mesh
{
private:
    //OpenGL buffers
    Ref<VertexArray>  vertexArray;
    Ref<VertexBuffer> vertexBuffer;
    Ref<IndexBuffer>  indexBuffer;

    //Memory buffers
    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;

public:
    Mesh();
    ~Mesh();

    const Ref<VertexArray> getVertexArray() const { return vertexArray; }
    const Ref<IndexBuffer> getIndexBuffer() const { return indexBuffer; }

    void loadOBJ(const std::string& path);

    //Manual data updates
    void setVertices(const std::vector<MeshVertex>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
    void setIndices(const std::vector<unsigned short>& indices);
    void setPositions(const std::vector<vec3>& positions);
    void setNormals(const std::vector<vec3>& normals);
    void setColors(const std::vector<vec3>& colors);
    void setUVs(const std::vector<vec2>& uvs);
};