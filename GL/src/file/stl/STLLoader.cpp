#include "STLLoader.h"

const std::vector<vec3>& STLLoader::getPositions() const
{
    return positions ; 
}

const std::vector<vec3>& STLLoader::getNormals() const 
{ 
    return normals;
}

bool STLLoader::onLoad(const std::string& path)
{
    std::ifstream stream(path);

    //Test for file opening errors
    if (!stream.is_open())
    {
        LogError("Failed to open STL File '{0}'.", path);
        return false;
    }

    positions.clear();
    normals.clear();

    bool result = parse(stream);

    stream.close();
    return result;
}

bool STLLoader::parse(std::ifstream& stream)
{
    //Ignore Header
    stream.ignore(80);

    //Read Triangle count
    unsigned int triangleCount;
    if (!(stream >> triangleCount))
    {
        LogError("Failed to read STL Triangle count.");
        return false;
    }

    normals.reserve(3 * triangleCount);
    positions.reserve(3 * triangleCount);


    //Read each Triangle data
    for (unsigned int triangle = 0; triangle < triangleCount; triangle++)
    {
        //Read the normal
        vec3 normal;
        stream >> normal.x >> normal.y >> normal.z;
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);

        //Read the positions
        vec3 position;
        
        stream >> position.x >> position.y >> position.z; 
        positions.push_back(position);
        
        stream >> position.x >> position.y >> position.z;
        positions.push_back(position);
        
        stream >> position.x >> position.y >> position.z; 
        positions.push_back(position);

        if (!stream)
        {
            LogError("Failed to parse STL Triangle");
            return false;
        }
    }
    return true;
}