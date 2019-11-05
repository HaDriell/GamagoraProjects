#pragma once

#include "../../Common.h"

#include "../Shader.h"
#include "../Buffer.h"
#include "../VertexArray.h"

#include "Renderable2D.h"

struct Vertex2D
{
    vec2    position;
    vec3    color;
    vec2    uv;
    float   textureID;
};

class Renderer2D
{
public:
    static Ref<Shader> LoadShader();

private:
    //Low level Renderer data structure
    Ref<VertexArray>                vertexArray;
    Ref<VertexBuffer>               vertexBuffer;
    Ref<IndexBuffer>                indexBuffer;
    Ref<Shader>                     shader;
    //Batch data
    unsigned int                    batchSize;
    unsigned int                    batchCapacity;
    std::vector<mat4>               transformationStack;
    std::vector<Vertex2D>           vertices;
    std::vector<unsigned int>       indices;
public:
    Renderer2D();

    //TODO : use materials instead when available. They will secure that the Shader resources are kept alive
    void begin(const mat4& root = mat4(), unsigned int batchCapacity = 10000);
    void end();

    void push(const mat4& matrix, bool absolute = false);
    void pop();

    void fillRect(float x, float y, float width, float height, const vec3& color = vec3(1));
    // void drawImage(float x, float y, float width, float height, const Texture& texture, const vec3& color = vec3(1));

    void draw(const Renderable2D& renderable);
};