#include "OBJLoader.h"


const std::vector<vec3>& OBJLoader::getPositions() const
{
    return positions;
}
const std::vector<vec3>& OBJLoader::getNormals() const
{
    return normals;
}

const std::vector<vec2>& OBJLoader::getUVs() const
{
    return uvs;
}

const std::vector<OBJFace>& OBJLoader::getFaces() const
{
    return faces;
}

bool OBJLoader::onLoad(const std::string& path)
{
    std::ifstream stream(path);

    //Test for file opening errors
    if (!stream.is_open())
    {
        LogError("Failed to open OBJ File '{0}'.", path);
        return false;
    }

    //Clean-up any previously parsed data
    faces.clear();
    positions.clear();
    normals.clear();
    uvs.clear();

    //Parse the file stream
    bool result = parse(stream);

    //Close the file
    stream.close();

    //Report
    return result;
}

bool OBJLoader::parse(std::ifstream& file)
{
    unsigned int lineNumber = 0;
    std::string line;
    while (std::getline(file >> std::ws, line))
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
            if (header == "v" && !parsePosition(stream)) 
            {
                LogError("Failed to load OBJ File (Line {0}) : Invalid Vertex data.", lineNumber);
                return false;
            }
            
            if (header == "vn" && !parseNormal(stream)) 
            {
                LogError("Failed to load OBJ File (Line {0}) : Invalid Normal data.", lineNumber);
                return false;
            }
            
            if (header == "vt" && !parseUV(stream)) 
            {
                LogError("Failed to load OBJ File (Line {0}) : Invalid UV data.", lineNumber);
                return false;
            }

            if (header == "f" && !parseFace(stream))
            {
                LogError("Failed to load OBJ File (Line {0}) : Invalid Face data.", lineNumber);
                return false;
            }
        }
    }
    return true;
}

bool OBJLoader::parsePosition(std::istringstream& stream)
{
    stream >> std::ws; // consume whitespaces
    vec3 v = vec3(0);
    if (!(stream >> v.x >> v.y >> v.z))
        return false;
    positions.push_back(v);
    return true;
}

bool OBJLoader::parseNormal(std::istringstream& stream)
{
    stream >> std::ws; // consume whitespaces
    vec3 v = vec3(0);
    if (!(stream >> v.x >> v.y >> v.z))
        return false;
    normals.push_back(v);
    return true;
}

bool OBJLoader::parseUV(std::istringstream& stream)
{
    stream >> std::ws; // consume whitespaces
    vec2 v = vec2(0);
    if (!(stream >> v.x >> v.y))
        return false;
    uvs.push_back(v);
    return true;
}

bool OBJLoader::parseFace(std::istringstream& stream)
{
    OBJFace face;

    //Parse each vertex individually
    std::string vertexStringData;
    while (std::getline(stream >> std::ws, vertexStringData, ' '))
    {
        std::istringstream vertexStream(vertexStringData);
        if (!parseFaceVertex(vertexStream, face))
            return false;
    }

    //Minimum 3 Vertices per face
    if (face.size() < 3)
        return false;

    faces.push_back(face);
    return true;
}

bool OBJLoader::parseFaceVertex(std::istringstream& stream, OBJFace& face)
{
    OBJVertex vertex;
    vertex.position = 0;
    vertex.normal   = 0;
    vertex.uv       = 0;

    //Read Positon
    if (!(stream >> vertex.position)) return false;
    //Skip separator
    if (!stream.eof() && stream.peek() == '/') stream.ignore();
    //Read UV (optional)
    if (!stream.eof() && stream.peek() != '/' && !(stream >> vertex.uv)) return false;
    //Skip separator
    if (!stream.eof() && stream.peek() == '/') stream.ignore();
    //Read Normal (optional)
    if (!stream.eof() && stream.peek() != '/' && !(stream >> vertex.normal)) return false;

    face.push_back(vertex);
    return true;
}