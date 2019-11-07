#pragma once

#include "../../Common.h"
#include "../FileLoader.h"

struct OBJVertex
{
    unsigned int position;
    unsigned int normal;
    unsigned int uv;
};

using OBJFace = std::vector<OBJVertex>;


class OBJLoader : public FileLoader
{
private:
    //Face Data
    std::vector<OBJFace> faces;

    //Vertex Data
    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> uvs;
public:
    const std::vector<vec3>& getPositions() const;
    const std::vector<vec3>& getNormals() const;
    const std::vector<vec2>& getUVs() const;
    const std::vector<OBJFace>& getFaces() const;

protected:
    bool onLoad(const std::string& path) override;

private:
    bool parse(std::ifstream& stream);
    bool parsePosition(std::istringstream& stream);
    bool parseNormal(std::istringstream& stream);
    bool parseUV(std::istringstream& stream);
    bool parseFace(std::istringstream& stream);
    bool parseFaceVertex(std::istringstream& stream, OBJFace& face);
};