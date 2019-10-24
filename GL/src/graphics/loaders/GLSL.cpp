#include "GLSL.h"

#include <glad/glad.h>
#include <fstream>
#include <string>

bool loadGLSLFile(const std::string& filename, ShaderSources& sources)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Failed to read GLSL file : File '" << filename << "' not found !" << std::endl;
        return false;
    }


    //Initialise reading
    sources.clear();
    unsigned int shaderSection = GL_FALSE;
    std::string line;

    //Read line by line
    while (std::getline(file, line))
    {
        //Special comment handling
        if (line.rfind("//TesselationControl Shader", 0) == 0)      { shaderSection = GL_TESS_CONTROL_SHADER; continue; }
        if (line.rfind("//TesselationEvaluation Shader", 0) == 0)   { shaderSection = GL_TESS_EVALUATION_SHADER; continue; }
        if (line.rfind("//Geometry Shader", 0) == 0)                { shaderSection = GL_GEOMETRY_SHADER ; continue; }
        if (line.rfind("//Vertex Shader", 0) == 0)                  { shaderSection = GL_VERTEX_SHADER; continue; }
        if (line.rfind("//Fragment Shader", 0) == 0)                { shaderSection = GL_FRAGMENT_SHADER; continue; }
        if (line.rfind("//Compute Shader", 0) == 0)                 { shaderSection = GL_COMPUTE_SHADER; continue; }

        //Not a Section declaration Comment
        if (shaderSection != GL_FALSE)
            sources[shaderSection] += line + "\n";
    }
    return true;
}

