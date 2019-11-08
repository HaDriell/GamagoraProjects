#pragma once

#include "../../Common.h"

struct PointLight
{
    vec3 position;
    float intensity;
    vec3 color;

    PointLight(const vec3& position, const vec3& color, float intensity) : position(position), color(color), intensity(intensity) {}
};

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};

struct SpotLight
{
    vec3 direction;
    float innerCutOff;
    float outerCutOff;
};

struct LightSetup
{
    std::vector<PointLight>       pointLights;
    std::vector<DirectionalLight> directionalLights;
    std::vector<SpotLight>        spotLights;
};