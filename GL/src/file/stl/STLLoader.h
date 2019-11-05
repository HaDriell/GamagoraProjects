#pragma once

#include "../../Common.h"
#include "../FileLoader.h"

class STLLoader : public FileLoader
{
private:

    //Vertex Data
    std::vector<vec3> positions;
    std::vector<vec3> normals;
public:
    const std::vector<vec3>& getPositions() const;
    const std::vector<vec3>& getNormals() const;
protected:
    bool onLoad(const std::string& path) override;

    bool parse(std::ifstream& stream);
};