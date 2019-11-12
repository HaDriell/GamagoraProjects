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
        // computeNormals(); // Is already done when converting from the OBJLoader
        computeTangents();
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

void Mesh::computeNormals()
{
    for (unsigned int i = 2; i < indices.size(); i += 3)
    {
        //Extract vertex indices
        unsigned int index0 = indices[i - 2];
        unsigned int index1 = indices[i - 1];
        unsigned int index2 = indices[i - 0];
        //Extract vertex Positions
        vec3 vertex0 = vertices[index0].position;
        vec3 vertex1 = vertices[index1].position;
        vec3 vertex2 = vertices[index2].position;
        //Compute Normal
        vec3 edge1 = vertex1 - vertex0;
        vec3 edge2 = vertex2 - vertex0;
        vec3 normal = vec3::cross(edge1, edge2).normalize();
        //Assign Normal to vertices
        vertices[index0].normal = normal;
        vertices[index1].normal = normal;
        vertices[index2].normal = normal;
    }
}

void Mesh::computeTangents()
{
    for (unsigned int i = 2; i < indices.size(); i += 3)
    {
        //Extract vertex indices
        unsigned int index0 = indices[i - 2];
        unsigned int index1 = indices[i - 1];
        unsigned int index2 = indices[i - 0];

        //Extract vertex Positions & UVs
        MeshVertex& vertex0 = vertices[index0];
        MeshVertex& vertex1 = vertices[index1];
        MeshVertex& vertex2 = vertices[index2];

        //Compute Edges
        vec3 posEdge1 = vertex1.position - vertex0.position;
        vec3 posEdge2 = vertex2.position - vertex0.position;
        vec2 uvEdge1 = vertex1.uv - vertex0.uv;
        vec2 uvEdge2 = vertex2.uv - vertex0.uv;

        //Compute Tangent & Bitangent
        float r = 1.0f / vec2::cross(uvEdge1, uvEdge2);
        vec3 tangent    = ((posEdge1 * uvEdge2.y - posEdge2 * uvEdge1.y) * r).normalize();
        vec3 bitangent  = ((posEdge2 * uvEdge1.x - posEdge1 * uvEdge2.x) * r).normalize();

        //Assign them back to the vertex
        vertex0.tangent = tangent;
        vertex1.tangent = tangent;
        vertex2.tangent = tangent;
        vertex0.bitangent = bitangent;
        vertex1.bitangent = bitangent;
        vertex2.bitangent = bitangent;
    }
}