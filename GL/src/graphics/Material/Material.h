#pragma once

#include "../../Common.h"
#include "../Shader.h"
#include "../Camera.h"

#include "Light.h"

class Material
{
protected:
    Ref<Shader> shader;
    // MAYDO : replace the default methods by a set of constants/refrences when using a custom material format

protected:
    Material(Ref<Shader> shader);

public:
    void bind();
    virtual void setupModelMatrix(const mat4& modelMatrix);   //Model Matrix
    virtual void setupCamera(const Camera& camera);           //View Matrix and Projection Matrix
    virtual void setupLights(const LightSetup& setup);        //Light Setup (optional)
    virtual void setupProperties() = 0;                       //Presets are for user defined resources (Textures, colors, etc...)
};