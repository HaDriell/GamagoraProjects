#pragma once

#include <memory>

#include "maths.h"

#include "Shader.h"
#include "VertexArray.h"

enum DepthTestMode
{
    Always,     Never,
    Equal,      NotEqual,
    Less,       LessOrEqual,
    Greater,    GreaterOrEqual,
};

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
    bool            depthWriting    = true;
    bool            depthTesting    = false;
    DepthTestMode   depthTestMode   = DepthTestMode::Less;
    //Face culling (culls back face)
    bool            faceCulling     = false;
    //Blending Configuration
    bool            blending        = false;
    BlendingFactor  srcFactor       = BlendingFactor::One;
    BlendingFactor  dstFactor       = BlendingFactor::Zero;
    BlendingMode    blendingMode    = BlendingMode::Add;
};


class Render
{
public:
    static void ClearError();
    static void CheckError();

    static void ConfigurePipeline(const RenderPipeline& pipeline);

    static void Init();
    static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    static void ClearColor(const vec4& color);
    static void Clear(bool color = true, bool depth = true, bool stencil = false);

    static void DrawIndexed(const VertexArray& vertexArray, const IndexBuffer&  indexBuffer);
};