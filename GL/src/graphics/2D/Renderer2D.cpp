#include "Renderer2D.h"

#include <string>
#include "../Render.h"



///////////////////////////////////////////////////////////////////////////////
// Renderer2D Rendering Pipeline //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static RenderPipeline CreateR2DPipeline()
{
    RenderPipeline pipeline;
    pipeline.blending = true;
    pipeline.blendingMode = BlendingMode::Add;
    pipeline.srcFactor = BlendingFactor::SrcAlpha;
    pipeline.dstFactor = BlendingFactor::OneMinusSrcAlpha;
    return pipeline;
}

const RenderPipeline R2DPipeline = CreateR2DPipeline();

const BufferLayout R2DLayout = BufferLayout{
    { VertexAttributeType::Float2, "Position"   },
    { VertexAttributeType::Float3, "Color"      },
    { VertexAttributeType::Float2, "UV"         },
    { VertexAttributeType::Float,  "TextureID"  },
};

const std::string R2DShader = "R2DShader";
const std::string R2DVertexShaderSource = R"(
    #version 450 core

    in vec2     Position;
    in vec3     Color;
    in vec2     UV;
    in float    TextureID;

    out vec2    vs_Position;
    out vec3    vs_Color;
    out vec2    vs_UV;
    out float   vs_TextureID;


    void main()
    {
        gl_Position     = vec4(Position, 0.0, 1.0);
        
        vs_Position     = Position;
        vs_Color        = Color;
        vs_UV           = UV;
        vs_TextureID    = TextureID;
    }
)";

const std::string R2DFragmentShaderSource = R"(
    #version 450 core

    in vec2     vs_Position;
    in vec3     vs_Color;
    in vec2     vs_UV;
    in float    vs_TextureID;

    out vec4 fs_Color;

    uniform sampler2D u_Texture[32];

    void main()
    {
        fs_Color = vec4(vs_Color, 1);
/*
        if (vs_TextureID > 0.0)
        {
            int TID = int(vs_TextureID);
            fs_Color *= texture(u_Texture[TID], vs_UV);
        }
*/
    }
)";


///////////////////////////////////////////////////////////////////////////////
// Renderer2D Functions ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Renderer2D::Renderer2D() : batchSize(0), batchCapacity(10000)
{
    //Use an interleaved VertexBuffer
    vertexBuffer    = std::make_shared<VertexBuffer>();
    vertexArray     = std::make_shared<VertexArray>();
    indexBuffer     = std::make_shared<IndexBuffer>();
    shader          = Renderer2D::LoadShader();

    //Bind VertexBuffer to VertexArray
    vertexBuffer->defineLayout(R2DLayout);
    vertexArray->addVertexBuffer(vertexBuffer);
}


Ref<Shader> Renderer2D::LoadShader()
{
    Ref<Shader> shader = Assets<Shader>::Find(R2DShader);
    if (!shader)
    {
        shader = MakeRef<Shader>();
        ShaderSources sources;
        sources[ShaderType::Vertex]     = R2DVertexShaderSource;
        sources[ShaderType::Fragment]   = R2DFragmentShaderSource; 
        shader->compile(sources);
        if (!shader->isValid())
            shader->debug();
        Assets<Shader>::Add(R2DShader, shader);
    }
    return shader;
}

void Renderer2D::begin(const mat4& root, unsigned int batchCapacity)
{
    //Clean any previous Batch
    if (batchSize > 0)
    {
        LogWarning("Forcing {0} Sprites to be rendered : Begining a new Batch.", batchSize);
        end();
    }

    batchSize = 0;
    this->batchCapacity = batchCapacity;

    //Setup the stack with the root as a base
    transformationStack.clear();
    transformationStack.push_back(root);

    //Setup the memory buffers
    vertices.clear();
    vertices.reserve(batchCapacity * 4);
}

void Renderer2D::end()
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
    shader->bind();
    Render::ConfigurePipeline(R2DPipeline);
    Render::DrawIndexed(*vertexArray, *indexBuffer);

    //Reset geometry buffer
    vertices.clear();
    batchSize = 0;
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
    Vertex2D vertex;

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
    if (batchSize >= batchCapacity) end();
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