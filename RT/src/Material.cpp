#include "Material.h"


Material Material::Diffuse(float opaqueness, float r, float g, float b)
{
    Material m;
    m.diffuse = true;
    m.opaqueness = opaqueness;
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


Material Material::Reflective(float reflectiveness)
{
    Material m;
    m.reflective = true;
    m.reflectiveness = reflectiveness;
    return m;
}

Material& operator|=(Material& a, const Material& b) 
{
    a = a | b;
    return a;
}

Material operator|(const Material& a, const Material& b)
{
    Material t;
    if (a.emissive)
    {
        t.emissive           = a.emissive;
        t.emission_intensity = a.emission_intensity;
        t.emission_color     = a.emission_color;
    }

    if (b.emissive)
    {
        t.emissive           = b.emissive;
        t.emission_intensity = b.emission_intensity;
        t.emission_color     = b.emission_color;        
    }

    if (a.diffuse)
    {
        t.diffuse    = a.diffuse;
        t.opaqueness = a.opaqueness;
        t.albedo     = a.albedo;
    }
    
    if (b.diffuse)
    {
        t.diffuse    = b.diffuse;
        t.opaqueness = b.opaqueness;
        t.albedo     = b.albedo;
    }

    if (a.reflective)
    {
        t.reflective     = a.reflective;
        t.reflectiveness = a.reflectiveness;
    }

    if (b.reflective)
    {
        t.reflective     = b.reflective;
        t.reflectiveness = b.reflectiveness;
    }

    return t;
}