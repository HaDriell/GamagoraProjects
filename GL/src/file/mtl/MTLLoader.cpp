#include "MTLLoader.h"

////////////////////////////////////////////////////////////////////////////////
// Helper functions to read data ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool __parseMTLString(std::istream& stream, std::string& string)
{
    return !std::getline(stream, string).bad();
}

bool __parseMTLScalar(std::istream& stream, float& scalar)
{
    return !(stream >> scalar).bad();
}

bool __parseMTLInteger(std::istream& stream, int& integer)
{
    return !(stream >> integer).bad();
}

bool __parseMTLVec3(std::istream& stream, vec3& color)
{
    return !(stream >> color.x >> color.y >> color.z).bad();
}

bool __parseMTLTexture(std::istream& stream, MTLTexture& texture)
{
    std::string key;
    while (std::getline(stream, key, ' '))
    {
        if (key == "-bm")
        {
            std::string value;
            if (!__parseMTLString(stream, value))
            {
                LogError("Failed to read MTL Texture option {0}.", key);
                return false;
            }
        }

        if (key == "-cc")
        {
            std::string value;
            if (!__parseMTLString(stream, value))
            {
                LogError("Failed to read MTL Texture option {0}.", key);
                return false;
            }
            else
            {
                texture.colorCorrection = value == "on";
            }
        }

        if (key == "-clamp")
        {
            std::string value;
            if (!__parseMTLString(stream, value))
            {
                LogError("Failed to read MTL Texture option {0}.", key);
                return false;
            }
            else
            {
                texture.clamp = value == "on";
            }
        }

        if (key == "-blendu")
        {
            std::string value;
            if (!__parseMTLString(stream, value))
            {
                LogError("Failed to read MTL Texture option {0}.", key);
                return false;
            }
        }

        if (key == "-blendv")
        {
            std::string value;
            if (!__parseMTLString(stream, value))
            {
                LogError("Failed to read MTL Texture option {0}.", key);
                return false;
            }
        }

        if (key == "-imfchan")
        {
            std::string value;
            if (!__parseMTLString(stream, value))
            {
                LogError("Failed to read MTL Texture option {0}.", key);
                return false;
            }
        }

        if (key == "-mm")
        {
            std::string base, gain;
            if (!__parseMTLString(stream, base) || !__parseMTLString(stream, gain))
            {
                LogError("Failed to read MTL Texture option {0}.", key);
                return false;
            }
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Parser Functions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool MTLLoader::onLoad(const std::string& path)
{
    std::ifstream stream(path);

    //Test for file opening errors
    if (!stream.is_open())
    {
        LogError("Failed to open MTL File '{0}'.", path);
        return false;
    }


    //Clean-up any previously parsed data
    materials.clear();

    //Setup the current working directory (for Texture map names)
    size_t i = path.rfind('/', path.length());
    if (i != std::string::npos)
    {
        cwd = path.substr(0, i);
    }
    else
    {
        LogWarning("Failed to identify the current working directory !");
        cwd = ".";
        //Continue because sometimes it's just simple materials & people won't care
    }

    //Parse the file stream
    bool result = parse(stream);

    //Close the file
    stream.close();

    return result;
}

bool MTLLoader::parse(std::istream& stream)
{
    unsigned int lineNumber = 0;
    std::string line;
    while (std::getline(stream >> std::ws, line))
    {
        lineNumber++;
        
        //Remove return carriage from Window encoded files
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        std::istringstream stream(line);
        std::string header;
        //Read the line header
        if (std::getline(stream >> std::ws, header, ' '))
        {
            if (header == "newmtl" && !parseMaterialName(stream))
            {
                LogError("Failed to load MTL File (Line {0}) : Invalid Material name", lineNumber);
                return false;
            }

            if (header == "Ka" && !parseMaterialAmbientColor(stream))
            {
                LogError("Failed to load MTL File (Line {0}) : Invalid Ambient Color", lineNumber);
                return false;
            }

            if (header == "Kd" && !parseMaterialDiffuseColor(stream))
            {
                LogError("Failed to load MTL File (Line {0}) : Invalid Diffuse Color", lineNumber);
                return false;
            }

            if (header == "Ks" && !parseMaterialSpecularColor(stream))
            {
                LogError("Failed to load MTL File (Line {0}) : Invalid Specular Color", lineNumber);
                return false;
            }

            if (header == "Tr" && !parseMaterialTransparency(stream))
            {
                LogError("Failed to load MTL File (Line {0}) : Invalid Transparency", lineNumber);
                return false;
            }

            if (header == "d" && !parseMaterialNonTransparency(stream))
            {
                LogError("Failed to load MTL File (Line {0}) : Invalid Non-Transparency", lineNumber);
                return false;
            }

            if (header == "illum" && !parseMaterialIlluminationModel(stream))
            {
                LogError("Failed to load MTL File (Line {0}) : Invalid Illumination model", lineNumber);
                return false;
            }

            if (header == "map_Ka" && !parseMaterialAmbientMap(stream))
            {
                LogError("Failed to load MTL File (Line {0}) : Invalid Ambient Map", lineNumber);
                return false;
            }

            if (header == "map_Kd" && !parseMaterialDiffuseMap(stream))
            {
                LogError("Failed to load MTL File (Line {0}) : Invalid Diffuse Map", lineNumber);
                return false;
            }

            if (header == "map_Ks" && !parseMaterialSpecularMap(stream))
            {
                LogError("Failed to load MTL File (Line {0}) : Invalid Specular Map", lineNumber);
                return false;
            }
        }
    }
    return true;
}

bool MTLLoader::parseMaterialName(std::istream& stream)
{
    std::string materialName;
    if (!__parseMTLString(stream >> std::ws, materialName))
        return false;
    materials.push_back(MTLMaterial());
    materials.back().name = materialName;
    return true;
}

bool MTLLoader::parseMaterialAmbientColor(std::istream& stream)
{
    if (materials.empty()) return false;
    return __parseMTLVec3(stream >> std::ws, materials.back().ambientColor);
}

bool MTLLoader::parseMaterialDiffuseColor(std::istream& stream)
{
    if (materials.empty()) return false;
    return __parseMTLVec3(stream >> std::ws, materials.back().diffuseColor);
}

bool MTLLoader::parseMaterialSpecularColor(std::istream& stream)
{
    if (materials.empty()) return false;
    return __parseMTLVec3(stream >> std::ws, materials.back().specularColor);
}

bool MTLLoader::parseMaterialTransparency(std::istream& stream)
{
    if (materials.empty()) return false;
    return __parseMTLScalar(stream >> std::ws, materials.back().transparency);
}

bool MTLLoader::parseMaterialNonTransparency(std::istream& stream)
{
    if (materials.empty()) return false;
    float nonTransparency;
    if (!__parseMTLScalar(stream >> std::ws, nonTransparency))
        return false;
    materials.back().transparency = 1 - nonTransparency;
    return true;
}

bool MTLLoader::parseMaterialIlluminationModel(std::istream& stream)
{
    if (materials.empty()) return false;
    return __parseMTLInteger(stream, materials.back().illumination);
}

bool MTLLoader::parseMaterialAmbientMap(std::istream& stream)
{
    if (materials.empty()) return false;
    return __parseMTLTexture(stream, materials.back().ambientMap);
}

bool MTLLoader::parseMaterialDiffuseMap(std::istream& stream)
{
    if (materials.empty()) return false;
    return __parseMTLTexture(stream, materials.back().diffuseMap);
}

bool MTLLoader::parseMaterialSpecularMap(std::istream& stream)
{
    if (materials.empty()) return false;
    return __parseMTLTexture(stream, materials.back().specularMap);
}

std::vector<MTLMaterial> MTLLoader::getMaterials() const
{
    return materials;
}