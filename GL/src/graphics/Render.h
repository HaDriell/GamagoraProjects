#pragma once

#include <memory>

#include "maths.h"

#include "Shader.h"
#include "VertexArray.h"

enum BlendingFactor
{
    Zero, One, 
    SrcColor, OneMinusSrcColor,
    DstColor, OneMinusDstColor, 
    SrcAlpha, OneMinusSrcAlpha,
    DstAlpha, OneMinusDstAlpha,
};

enum BlendingMode
{
    Min, Max, Add, Subtract, ReverseSubtract
};

struct RenderPipeline
{
    //Depth Testing
    bool            depthTesting    = false;
    bool            faceCulling     = false;

    //Blending Configuration
    bool            blending        = false;
    BlendingFactor  srcBlending     = BlendingFactor::SrcAlpha;
    BlendingFactor  dstBlending     = BlendingFactor::OneMinusSrcAlpha;
    BlendingMode    blendingMode    = BlendingMode::Add;
};


class Render
{
public:
    static void Debug();

    static void ConfigurePipeline(const RenderPipeline& pipeline);

    static void Init();
    static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    static void ClearColor(const vec4& color);
    static void Clear();

    static void DrawIndexed(const VertexArray& vertexArray, const IndexBuffer&  indexBuffer);
};