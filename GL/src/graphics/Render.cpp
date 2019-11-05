#include "Render.h"

#include <glad/glad.h>
#include <iostream>


GLenum toOpenGLBlendFactor(BlendingFactor factor)
{
    switch (factor)
    {
        case Zero:              return GL_ZERO;
        case One:               return GL_ONE;
        case SrcColor:          return GL_SRC_ALPHA;
        case DstColor:          return GL_DST_ALPHA;
        case SrcAlpha:          return GL_SRC_COLOR;
        case DstAlpha:          return GL_DST_COLOR;
        case OneMinusSrcAlpha:  return GL_ONE_MINUS_SRC_ALPHA;
        case OneMinusDstAlpha:  return GL_ONE_MINUS_DST_ALPHA;
        case OneMinusSrcColor:  return GL_ONE_MINUS_SRC_COLOR;
        case OneMinusDstColor:  return GL_ONE_MINUS_DST_COLOR;
    }
    return GL_FALSE;
}

GLenum toOpenGLBlendMode(BlendingMode mode)
{
    switch (mode)
    {
        case Min:               return GL_MIN;
        case Max:               return GL_MAX;
        case Add:               return GL_FUNC_ADD;
        case Subtract:          return GL_FUNC_SUBTRACT;
        case ReverseSubtract:   return GL_FUNC_REVERSE_SUBTRACT;
    }
    return GL_FALSE;
}

RenderPipeline::RenderPipeline(bool depthTesting, bool blending, BlendingMode blendingMode, BlendingFactor sourceFactor, BlendingFactor destinationFactor, bool faceCulling)
:   depthTesting(depthTesting), blending(blending), blendingMode(blendingMode), srcFactor(sourceFactor), dstFactor(destinationFactor), faceCulling(faceCulling)
{
}

void Render::Debug()
{
    unsigned int error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error("<< error << ")" << std::endl;
    }
}

void Render::ConfigurePipeline(const RenderPipeline& pipeline)
{
    if (!pipeline.blending)
    {
        glDisable(GL_BLEND);
    }
    else
    {
        glEnable(GL_BLEND);
        glBlendEquation(toOpenGLBlendMode(pipeline.blendingMode));
        glBlendFunc(toOpenGLBlendFactor(pipeline.srcFactor), toOpenGLBlendFactor(pipeline.dstFactor));
    }

    if (!pipeline.depthTesting)
    {
        glDisable(GL_DEPTH_TEST);
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
    }

    if (!pipeline.faceCulling)
    {
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
}

void Render::Init()
{
    glEnable(GL_DEPTH_TEST);
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

void Render::DrawIndexed(const VertexArray& vertexArray, const IndexBuffer& indexBuffer)
{
    vertexArray.bind();
    indexBuffer.bind();
    glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), indexBuffer.getFormat(), nullptr);
}