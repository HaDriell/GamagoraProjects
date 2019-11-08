#include "Mesh.h"
#include "../../File.h"

#include <glad/glad.h>

Mesh::Mesh()
{
    //Create all OpenGL buffers associated with that mesh
    vertexArray  = std::make_shared<VertexArray>();
    vertexBuffer = std::make_shared<VertexBuffer>();
    indexBuffer  = std::make_shared<IndexBuffer>();

    //Setup the layout
    vertexBuffer->defineLayout(MeshVertexLayout);
    vertexArray->addVertexBuffer(vertexBuffer);
}

Mesh::~Mesh()
{
}

void Mesh::loadOBJ(const std::string& path)
{
    OBJLoader loader;
    if (loader.load(path))
    {
        vertices.clear();
        indices.clear();
        unsigned int index = 0;
        for (const OBJFace& face : loader.getFaces())
        {
            unsigned int fan = 2;
            while (fan < face.size())
            {
                OBJVertex ov0 = face[0];
                OBJVertex ov1 = face[fan - 1];
                OBJVertex ov2 = face[fan];
                vec3 p0 = loader.getPositions()[ov0.position - 1];
                vec3 p1 = loader.getPositions()[ov1.position - 1];
                vec3 p2 = loader.getPositions()[ov2.position - 1];
                vec3 edge1 = p1 - p0;
                vec3 edge2 = p2 - p0;
                vec3 default_normal = vec3::cross(edge1, edge2).normalize();
                vec2 default_uv = vec2(0);

                MeshVertex meshVertex;
                meshVertex.position = p0;
                meshVertex.normal   = ov0.normal ? loader.getNormals()[ov0.normal - 1] : default_normal;
                meshVertex.color    = vec3(1);
                meshVertex.uv       = ov0.uv ? loader.getUVs()[ov0.uv - 1] : default_uv;
                vertices.push_back(meshVertex);
                indices.push_back(index++);

                meshVertex.position = p1;
                meshVertex.normal   = ov1.normal ? loader.getNormals()[ov1.normal - 1] : default_normal;
                meshVertex.color    = vec3(1);
                meshVertex.uv       = ov1.uv ? loader.getUVs()[ov1.uv - 1] : default_uv;
                vertices.push_back(meshVertex);
                indices.push_back(index++);

                meshVertex.position = p2;
                meshVertex.normal   = ov2.normal ? loader.getNormals()[ov2.normal - 1] : default_normal;
                meshVertex.color    = vec3(1);
                meshVertex.uv       = ov2.uv ? loader.getUVs()[ov2.uv - 1] : default_uv;
                vertices.push_back(meshVertex);
                indices.push_back(index++);

                fan++;
            }
        }
        setVertices(vertices);
        setIndices(indices);
        LogDebug("Mesh loaded from File '{0}'. {1} Vertices. {2} Indices.", path, vertices.size(), indices.size());
    }
}

void Mesh::setVertices(const std::vector<MeshVertex>& vertices)
{
    this->vertices = vertices;
    vertexBuffer->defineData(this->vertices);
}

void Mesh::setIndices(const std::vector<unsigned int>& indices)
{
    this->indices = indices;
    indexBuffer->defineData(this->indices);
}

void Mesh::setIndices(const std::vector<unsigned short>& indices)
{
    this->indices.resize(indices.size());
    for(unsigned int i = 0; i < indices.size(); i++)
    {
        this->indices[i] = indices[i];
    }
    indexBuffer->defineData(this->indices);
}

void Mesh::setPositions(const std::vector<vec3>& positions)
{
    this->vertices.resize(positions.size());
    for (unsigned int i = 0; i < positions.size(); i++)
    {
        this->vertices[i].position = positions[i];
    }
    vertexBuffer->defineData(this->vertices);
}

void Mesh::setNormals(const std::vector<vec3>& normals)
{
    this->vertices.resize(normals.size());
    for (unsigned int i = 0; i < normals.size(); i++)
    {
        this->vertices[i].normal = normals[i];
    }
    vertexBuffer->defineData(this->vertices);
}

void Mesh::setColors(const std::vector<vec3>& colors)
{
    this->vertices.resize(colors.size());
    for (unsigned int i = 0; i < colors.size(); i++)
    {
        this->vertices[i].color = colors[i];
    }
    vertexBuffer->defineData(this->vertices);
}

void Mesh::setUVs(const std::vector<vec2>& uvs)
{
    this->vertices.resize(uvs.size());
    for (unsigned int i = 0; i < uvs.size(); i++)
    {
        this->vertices[i].uv = uvs[i];
    }
    vertexBuffer->defineData(this->vertices);
}