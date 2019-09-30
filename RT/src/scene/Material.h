#pragma once

#include "../math/math.h"

//By default a constructed material is an opaque white material
struct Material
{
    //Emissive Material properties (Light emission stuff)
    bool    emissive            = false;
    float   emission_intensity  = 0;
    vec3    emission_color      = vec3(0, 0, 0);

    //Diffuse Material properties (opaque stuff that directly reflects from light sources)
    bool    diffuse     = false;
    vec3    albedo      = vec3(0, 0, 0);

    static Material Diffuse(float r, float g, float b);
    static Material Emissive(float intensity, float r, float g, float b);
};

//TODO : make a "merge" Material so that they can be merged