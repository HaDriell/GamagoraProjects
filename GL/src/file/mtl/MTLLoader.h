#pragma once

#include "../../Common.h"
#include "../FileLoader.h"

/*
Illuminiation models:
    0 Color on and Ambient off
    1 Color on and Ambient on
    2 Highlight on
    3 Reflection on and Ray trace on
    4 Transparency: Glass on
    Reflection: Ray trace on
    5 Reflection: Fresnel on and Ray trace on
    6 Transparency: Refraction on
    Reflection: Fresnel off and Ray trace on
    7 Transparency: Refraction on
    Reflection: Fresnel on and Ray trace on
    8 Reflection on and Ray trace off
    9 Transparency: Glass on
    Reflection: Ray trace off
    10 Casts shadows onto invisible surfaces
*/

struct MTLTexture
{
    std::string path;
    bool        clamp           = true;
    bool        colorCorrection = false;
    //bm is not supported     (1.0 by default)
    //blendu is not supported (on by default)
    //blendv is not supported (on by default)
    //boost is not supported (specific to a renderer ?)
    //imfchan is not supported (yet, it's used to know which channel should be read for transparency, specular exponent, decal, displacement maps)
    //mm not supported
    vec3        offset          = vec3(0);
    vec3        size            = vec3(1);
    //turbulence is not supported (I don't know how to implement turbulence on textures)
    //texres is unsupported
};

struct MTLMaterial
{
    //Name
    std::string name;
    //Colors
    vec3        ambientColor   = vec3(0.2f, 0.2f, 0.2f);
    vec3        diffuseColor   = vec3(0.8f, 0.8f, 0.8f);
    vec3        specularColor  = vec3(1.0f, 1.0f, 1.0f);
    //Params
    float       transparency  = 0.0f;
    float       shininess     = 0.0f;
    //Illumination model
    int        illumination;
    //Textures
    MTLTexture ambientMap;
    MTLTexture diffuseMap;
    MTLTexture specularMap;
    //map_Ns unsupported
    //map_d unsupported
    //map_aat unsupported
    //decal unsupported
};

// TODO : https://fr.wikipedia.org/wiki/Material_Template_Library

class MTLLoader : public FileLoader
{
private:
    std::vector<MTLMaterial> materials;
    std::string cwd;

public:
    std::vector<MTLMaterial> getMaterials() const;

protected:
    bool onLoad(const std::string& path) override;

private:
    bool parse(std::istream& stream);

    bool parseMaterialName(std::istream& stream);
    bool parseMaterialAmbientColor(std::istream& stream);
    bool parseMaterialDiffuseColor(std::istream& stream);
    bool parseMaterialSpecularColor(std::istream& stream);
    bool parseMaterialTransparency(std::istream& stream);
    bool parseMaterialNonTransparency(std::istream& stream);
    bool parseMaterialIlluminationModel(std::istream& stream);
    bool parseMaterialAmbientMap(std::istream& stream);
    bool parseMaterialDiffuseMap(std::istream& stream);
    bool parseMaterialSpecularMap(std::istream& stream);

};