#include "Material.h"


Material::Material(Ref<Shader> shader) : shader(shader) {}


void Material::bind()
{
    shader->bind();
}

void Material::setupModelMatrix(const mat4& modelMatrix)
{
}

void Material::setupCamera(const Camera& camera)
{
}

void Material::setupLights(const LightSetup& setup)
{
}