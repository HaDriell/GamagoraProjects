#include "Renderer2D.h"

#include <string>

#include "../Render.h"


#define MAX_SPRITES     60000
#define MAX_VERTICES    MAX_SPRITES * 4
#define MAX_INDICES     MAX_SPRITES * 6

Renderer2D::Renderer2D()
{
    //Use an interleaved VertexBuffer
    vertexBuffer    = std::make_shared<VertexBuffer>();
    vertexArray     = std::make_shared<VertexArray>();
    indexBuffer     = std::make_shared<IndexBuffer>();

    //Bind VertexBuffer to VertexArray
    vertexBuffer->defineLayout(Renderer2DVertexLayout);
    vertexArray->addVertexBuffer(vertexBuffer);
}

void Renderer2D::begin(Ref<Shader> activeShader, const mat4& root, unsigned int batchCapacity)
{
    //Clean any previous Batch
    if (batchSize != 0) end();

    //Setup the shader reference
    shader = activeShader;

    //Setup the stack with the root as a base
    transformationStack.clear();
    transformationStack.push_back(root);

    //Setup the memory buffers
    vertices.clear();
    vertices.reserve(batchCapacity * 4);

}

void Renderer2D::end(bool restart)
{
    //Protect against empty Batches
    if (batchSize == 0) return;

    //Generate index memory buffer (assuming it is a batch of quads)
    indices.resize(batchSize * 6);
    for (unsigned int offset = 0, index  = 0; offset < indices.size(); offset += 6, index += 4)
    {
        //First quad triangle
        indices[offset + 0] = index + 0;
        indices[offset + 1] = index + 1;
        indices[offset + 2] = index + 2;
        //Second quad triangle
        indices[offset + 3] = index + 2;
        indices[offset + 4] = index + 3;
        indices[offset + 5] = index + 0;
    }

    //Upload memory buffers
    vertexBuffer->defineData(vertices);
    indexBuffer->defineData(indices);

    
    //Do Indexed Draw Call
    RenderPipeline pipeline;
    pipeline.blending = true;
    pipeline.blendingMode = BlendingMode::Add;
    pipeline.srcBlending = BlendingFactor::SrcAlpha;
    pipeline.dstBlending = BlendingFactor::OneMinusSrcAlpha;
    Render::ConfigurePipeline(pipeline);
    shader->bind();
    Render::DrawIndexed(*vertexArray, *indexBuffer);

    //Reset geometry buffer
    vertices.clear();
    batchSize = 0;

    if (!restart)
    {
        shader.reset(); // drop Shader reference
    }
}

void Renderer2D::push(const mat4& matrix, bool absolute)
{
    if (absolute)
        transformationStack.push_back(transformationStack.front() * matrix);
    else
        transformationStack.push_back(transformationStack.back() * matrix);
}

void Renderer2D::pop()
{
    if (transformationStack.size() > 1)
    {
        transformationStack.pop_back();
    }
}

void Renderer2D::fillRect(float x, float y, float width, float height, const vec3& color)
{
    mat4 transform = transformationStack.back();
    Renderer2DVertex vertex;

    vertex.position     = transform * vec2(x, y);
    vertex.color        = color;
    vertex.uv           = vec2(0);
    vertex.textureID    = 0;
    vertices.push_back(vertex);

    vertex.position     = transform * vec2(x + width, y);
    vertex.color        = color;
    vertex.uv           = vec2(0);
    vertex.textureID    = 0;
    vertices.push_back(vertex);

    vertex.position     = transform * vec2(x + width, y + height);
    vertex.color        = color;
    vertex.uv           = vec2(0);
    vertex.textureID    = 0;
    vertices.push_back(vertex);

    vertex.position     = transform * vec2(x, y + height);
    vertex.color        = color;
    vertex.uv           = vec2(0);
    vertex.textureID    = 0;
    vertices.push_back(vertex);

    batchSize++;
    if (batchSize >= MAX_SPRITES) end(true);
}

// void Renderer2D::drawImage(float x, float y, float width, float height, const Texture& texture, const vec3& color)
// {
//     mat4 transform = transformationStack.back();

//     std::vector<R2DVertex> vertices;
//     R2DVertex vertex;

//     vertex.position = transform * vec3(x, y, 0);
//     vertex.color    = color;
//     vertex.uv       = vec2(0, 0);
//     vertices.push_back(vertex);

//     vertex.position = transform * vec3(x + width, y, 0);
//     vertex.color    = color;
//     vertex.uv       = vec2(1, 0);
//     vertices.push_back(vertex);

//     vertex.position = transform * vec3(x + width, y + height, 0);
//     vertex.color    = color;
//     vertex.uv       = vec2(1, 1);
//     vertices.push_back(vertex);

//     vertex.position = transform * vec3(x, y + height, 0);
//     vertex.color    = color;
//     vertex.uv       = vec2(0, 1);
//     vertices.push_back(vertex);

//     //Quad indices
//     std::vector<unsigned int> indices;
//     //T1
//     indices.push_back(0);
//     indices.push_back(1);
//     indices.push_back(2);
//     //T2
//     indices.push_back(2);
//     indices.push_back(3);
//     indices.push_back(0);

//     //Upload Data
//     vertexBuffer->defineData(vertices);
//     indexBuffer->defineData(indices);

//     //Draw Call
//     texture.bind();
//     shader->bind();
//     shader->setUniform("u_ProjectionMatrix", projectionMatrix);
//     shader->setUniform("u_Texture", 0);
//     Render::DrawIndexed(*vertexArray, *indexBuffer);
// }



void Renderer2D::draw(const Renderable2D& renderable)
{
    renderable.render(*this);
}