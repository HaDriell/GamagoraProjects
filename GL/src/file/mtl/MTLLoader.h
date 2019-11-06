#pragma once

#include "../../Common.h"
#include "../FileLoader.h"

struct MTLMaterial
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float transparency;

    unsigned int illumination;

    std::string ambientMap;
    std::string diffuseMap;
    std::string specularMap;
};

// TODO : https://fr.wikipedia.org/wiki/Material_Template_Library

class MTLLoader : public FileLoader
{
private:

public:
};