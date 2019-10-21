#include "Render.h"

#include <glad/glad.h>
#include <iostream>

void Render::Debug()
{
    unsigned int error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error("<< error << ")" << std::endl;
    }
}

void Render::Init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Render::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    glViewport(x, y, width, height);
}

void Render::ClearColor(const vec4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
}

void Render::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    vertexArray->bind();
    indexBuffer->bind();
    glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
}