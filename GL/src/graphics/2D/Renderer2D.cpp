#include "Renderer2D.h"

#include <string>

#include "../Render.h"

#include "../loaders/GLSL.h"

const BufferLayout R2D_INTERLEAVED_LAYOUT = {
    { VertexAttributeType::Float3, "vertex_position" },
    { VertexAttributeType::Float3, "vertex_color"    },
    { VertexAttributeType::Float2, "vertex_uv"       },
};

struct R2DVertex
{
    vec3 position;
    vec3 color;
    vec2 uv;
};


Renderer2D::Renderer2D(unsigned int width, unsigned int height) : width(width), height(height)
{
    //Load Identity View
    transformationStack.push_back(mat4());
    //Load top-left Orthographic Projection
    projectionMatrix = mat4::Orthographic(0, width, 0, height, 1, -1); // Z e [-1; 1]

    //Use an interleaved VertexBuffer
    vertexBuffer    = std::make_shared<VertexBuffer>();
    vertexArray     = std::make_shared<VertexArray>();
    indexBuffer     = std::make_shared<IndexBuffer>();
    shader          = std::make_shared<Shader>();
    //Bind VertexBuffer to VertexArray
    vertexBuffer->defineLayout(R2D_INTERLEAVED_LAYOUT);
    vertexArray->addVertexBuffer(vertexBuffer);    
    //Compile the Shader (TODO : find a clean way to embed the Shader inside GL !)
    ShaderSources sources;
    loadGLSLFile("res/2D/R2DShader.glsl", sources);
    shader->compile(sources);
    if (!shader->isValid()) 
        shader->debug();
}

void Renderer2D::pop()
{
    if (transformationStack.size() > 1)
    {
        transformationStack.pop_back();
    }
}

void Renderer2D::push(const mat4& matrix, bool absolute)
{
    if (absolute)
        transformationStack.push_back(matrix);
    else
        transformationStack.push_back(transformationStack.back() * matrix);
}

void Renderer2D::fillRect(float x, float y, float width, float height, const vec3& color)
{
    mat4 transform = transformationStack.back();

    std::vector<R2DVertex> vertices;
    R2DVertex vertex;

    vertex.position = transform * vec3(x, y, 0);
    vertex.color    = color;
    vertex.uv       = vec2(0);
    vertices.push_back(vertex);

    vertex.position = transform * vec3(x + width, y, 0);
    vertex.color    = color;
    vertex.uv       = vec2(0);
    vertices.push_back(vertex);

    vertex.position = transform * vec3(x + width, y + height, 0);
    vertex.color    = color;
    vertex.uv       = vec2(0);
    vertices.push_back(vertex);

    vertex.position = transform * vec3(x, y + height, 0);
    vertex.color    = color;
    vertex.uv       = vec2(0);
    vertices.push_back(vertex);

    //Quad indices
    std::vector<unsigned int> indices;
    //T1
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    //T2
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    //Upload Data
    vertexBuffer->defineData(vertices);
    indexBuffer->defineData(indices);

    //Draw Call
    shader->bind();
    shader->setUniform("ProjectionMatrix", projectionMatrix);
    Render::DrawIndexed(*vertexArray, *indexBuffer);
}

void Renderer2D::draw(const Renderable2D& renderable)
{
    renderable.render(*this);
}