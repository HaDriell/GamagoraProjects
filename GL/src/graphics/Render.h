#pragma once

#include <memory>

#include "maths.h"

#include "Shader.h"
#include "VertexArray.h"


class Render
{
public:
    static void Debug();

    static void Init();
    static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    static void ClearColor(const vec4& color);
    static void Clear();

    static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<IndexBuffer>& indexBuffer);
};