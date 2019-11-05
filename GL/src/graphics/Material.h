#pragma once

#include "../Common.h"
#include "Shader.h"
#include "Texture.h"
#include "Render.h"


class Material
{
private:
    Ref<Shader> shader;
    RenderPipeline pipeline;

public:
    Material(Ref<Shader> shader, const RenderPipeline& pipeline = RenderPipeline());
    
     // TODO : add a system binding to automatically upload a Projection, View, Model Matrix etc...
    void bind();
    void unbind();

protected:
    virtual void onBind(Ref<Shader> shader) = 0;
};