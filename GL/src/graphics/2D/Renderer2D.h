#pragma once

#include "../../Common.h"

#include "../Shader.h"
#include "../Buffer.h"
#include "../VertexArray.h"
#include "../Render.h"

#include "Renderable2D.h"


const RenderPipeline Renderer2DPipeline = {
    false, //Depth Testing
    true,  //Blending
};

const BufferLayout Renderer2DVertexLayout = BufferLayout{
    { VertexAttributeType::Float2, "Position"   },
    { VertexAttributeType::Float3, "Color"      },
    { VertexAttributeType::Float2, "UV"         },
    { VertexAttributeType::Float,  "TextureID"  },
};

struct Renderer2DVertex
{
    vec2    position;
    vec3    color;
    vec2    uv;
    float   textureID;
};


class Renderer2D
{
private:
    //Low level Renderer data structure
    Ref<VertexArray>                vertexArray;
    Ref<VertexBuffer>               vertexBuffer;
    Ref<IndexBuffer>                indexBuffer;
    Ref<Shader>                     shader;
    //Batch data
    unsigned int                    batchSize;
    std::vector<mat4>               transformationStack;
    std::vector<Renderer2DVertex>   vertices;
    std::vector<unsigned int>       indices;
public:
    Renderer2D();

    //TODO : use materials instead when available. They will secure that the Shader resources are kept alive
    void begin(Ref<Shader> shader, const mat4& root = mat4(), unsigned int batchCapacity = 10000);
    void end(bool restart = false);

    void push(const mat4& matrix, bool absolute = false);
    void pop();

    void fillRect(float x, float y, float width, float height, const vec3& color = vec3(1));
    // void drawImage(float x, float y, float width, float height, const Texture& texture, const vec3& color = vec3(1));

    void draw(const Renderable2D& renderable);
};