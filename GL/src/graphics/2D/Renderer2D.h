#pragma once

#include <memory>
#include <vector>

#include "maths.h"

#include "../Shader.h"
#include "../Buffer.h"
#include "../VertexArray.h"
#include "Renderable2D.h"

class Renderer2D
{
private:
    //Low level Renderer data structure
    std::shared_ptr<VertexArray>  vertexArray;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer>  indexBuffer;
    std::shared_ptr<Shader>       shader;

    //Transform
    std::vector<mat4>   transformationStack;
    mat4                projectionMatrix;

    //Surface info
    unsigned int        width;
    unsigned int        height;

public:
    Renderer2D(unsigned int width, unsigned int height);

    void push(const mat4& matrix, bool absolute = false);
    void pop();

    void fillRect(float x, float y, float width, float height, const vec3& color = vec3(1));

    void draw(const Renderable2D& renderable);
};