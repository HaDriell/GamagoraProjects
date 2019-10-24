#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh()
{
    //Create all OpenGL buffers associated with that mesh
    vertexArray = std::make_shared<VertexArray>();
    positions   = std::make_shared<VertexBuffer>(); 
    normals     = std::make_shared<VertexBuffer>(); 
    colors      = std::make_shared<VertexBuffer>();
    uvs         = std::make_shared<VertexBuffer>();
    indices     = std::make_shared<IndexBuffer>();

    //Setup the layout
    positions->defineLayout({{ VertexAttributeType::Float3, "position" }});
    normals->defineLayout({{ VertexAttributeType::Float3, "normal" }});
    colors->defineLayout({{ VertexAttributeType::Float3, "color" }});
    uvs->defineLayout({{ VertexAttributeType::Float2, "uv" }});

    //Configure the bindings
    vertexArray->addVertexBuffer(positions);
    vertexArray->addVertexBuffer(normals);
    vertexArray->addVertexBuffer(colors);
    vertexArray->addVertexBuffer(uvs);
}

Mesh::~Mesh()
{
    //shared_ptr are going to destroy themselves whenever they need to
}

void Mesh::setIndices(const std::vector<unsigned int>& indices)
{
    this->indices->defineData(indices);
}

void Mesh::setIndices(const std::vector<unsigned short>& indices)
{
    this->indices->defineData(indices);
}

void Mesh::setPositions(const std::vector<vec3>& positions)
{
    this->positions->defineData(positions);
}

void Mesh::setNormals(const std::vector<vec3>& normals)
{
    this->normals->defineData(normals);
}

void Mesh::setColors(const std::vector<vec4>& colors)
{
    this->colors->defineData(colors);
}

void Mesh::setUVs(const std::vector<vec2>& uvs)
{
    this->uvs->defineData(uvs);
}