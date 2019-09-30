#include "Material.h"


Material Material::Diffuse(float r, float g, float b)
{
    Material m;
    m.diffuse = true;
    m.albedo = vec3(r, g, b);
    return m;
}

Material Material::Emissive(float intensity, float r, float g, float b)
{
    Material m;
    m.emissive = true;
    m.emission_intensity = intensity;
    m.emission_color = vec3(r, g, b);
    return m;
}