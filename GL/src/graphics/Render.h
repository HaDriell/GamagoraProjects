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
    bool            depthTesting;
    //Face culling (culls back face)
    bool            faceCulling;
    //Blending Configuration
    bool            blending;
    BlendingFactor  srcFactor;
    BlendingFactor  dstFactor;
    BlendingMode    blendingMode;

    RenderPipeline( bool depthTesting = false, 
                    bool blending = false, 
                    BlendingMode blendingMode = BlendingMode::Add,
                    BlendingFactor sourceFactor = BlendingFactor::One,
                    BlendingFactor destinationFactor = BlendingFactor::Zero, 
                    bool faceCulling = false);
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