#pragma once

#include "../../Common.h"

#include "../Texture.h"

#include "Material.h"

class PhongMaterial : public Material
{
private:
    static Ref<Shader> LoadShader();

private:
    vec4 emissive;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;

public:
    PhongMaterial(const vec4& emissive, const vec4& ambient, const vec4& diffuse, const vec4& specular, float shininess);

    virtual void setupModelMatrix(const mat4& modelMatrix) override;
    virtual void setupCamera(const Camera& camera) override;
    virtual void setupLights(const LightSetup& setup) override;
    virtual void setupProperties() override;

    inline vec4 getEmissive() { return emissive; }
    inline vec4 getAmbient() { return ambient; }
    inline vec4 getDiffuse() { return diffuse; }
    inline vec4 getSpecular() { return specular; }
    inline float getShininess() { return shininess; }

    void setEmissive(const vec4& color);
    void setAmbient(const vec4& color);
    void setDiffuse(const vec4& color);
    void setSpecular(const vec4& color);
    void setShininess(float shininess);
};