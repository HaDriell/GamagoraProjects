#include "Shader.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <glad/glad.h>

std::string getShaderTypeString(GLenum shaderType)
{
    switch (shaderType)
    {
        case GL_TESS_CONTROL_SHADER:    return "TesselationControl";
        case GL_TESS_EVALUATION_SHADER: return "TesselationEvaluation";
        case GL_GEOMETRY_SHADER:        return "Geometry";
        case GL_VERTEX_SHADER:          return "Vertex";
        case GL_FRAGMENT_SHADER:        return "Fragment";
        case GL_COMPUTE_SHADER:         return "Compute";
    }
    return "Unknown";
}

Shader::Shader() : handle(0) 
{
    handle = glCreateProgram();
}

Shader::~Shader()
{
    glDeleteProgram(handle);
}

void Shader::debug() const
{
    std::cout << "Shader debug" << std::endl;

    if (!isLinked())
    {
        std::cout << "Program isn't linked" << std::endl;
        bool linkFailed = true;
        for (auto& entry : compilationStatus)
        {
            const std::string& shaderName   = entry.first;
            bool compiled                   = compilationStatus.at(shaderName);
            const std::string& info_log     = compilationLog.at(shaderName);
            //Log errors
            std::cout << shaderName << " Shader (" << (entry.second ? " OK " : "FAIL") << ")"
            << " : " << (info_log.empty() ? "No logs " : info_log) << std::endl;
            //Check for actual linking error
            if (linkFailed) linkFailed = compiled;
        }

        if (linkFailed)
        {
            std::cout << "Linking Failed : " << linkingLog << std::endl;
        }
    }

    if (!isValid())
    {
        std::cout << "Program isn't valid" << std::endl;
        std::cout << "Program Linking Log    : " << linkingLog << std::endl;
        std::cout << "Program Validation Log : " << validationLog << std::endl;
    }

    std::cin.get();
}

bool Shader::compile(const ShaderSources& shaderSources)
{
    //Clear metadata
    compilationLog.clear();
    compilationStatus.clear();
    linked = false;
    valid = false;

    //Compile each shader
    std::vector<unsigned int> shaders;
    for (auto& shaderSource : shaderSources)
    {
        unsigned int shaderType = shaderSource.first;
        const GLchar* source    = shaderSource.second.c_str();

        //Create Shader
        unsigned int shader     = glCreateShader(shaderType);
        shaders.push_back(shader);

        //Compile Shader
        glShaderSource(shader, 1, &source, 0);
        glCompileShader(shader);
        //Fetch Compilation info
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> info_log = std::vector<GLchar>(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &info_log[0]);
            //Fetch Compilation Status
            GLint isCompiled = GL_FALSE;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            
            //Store logs & compile status into the shader
            std::string shaderTypeString = getShaderTypeString(shaderType);
            compilationLog[shaderTypeString]    = std::string(info_log.begin(), info_log.end());
            compilationStatus[shaderTypeString] = isCompiled == GL_TRUE;
        }
    }

    //Check for successful shader compilations
    bool canLinkProgram = true;
    for (auto& shaderCompilation : compilationStatus)
    {
        if (!shaderCompilation.second)
        {
            canLinkProgram = false;
            break;
        }
    }

    //Proceed to linking only when shaders compiled
    if (canLinkProgram)
    {
        //Attach shaders to program
        for (unsigned int shader : shaders)
        {
            glAttachShader(handle, shader);
        }

        //Link Program
        glLinkProgram(handle);
        //Fetch linking info
        {
            GLint isLinked = GL_FALSE;
            glGetProgramiv(handle, GL_LINK_STATUS, &isLinked);
            GLint maxLength = 0;
            glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> info_log = std::vector<GLchar>(maxLength);
            glGetProgramInfoLog(handle, maxLength, &maxLength, &info_log[0]);

            linkingLog  = std::string(info_log.begin(), info_log.end());
            linked      = isLinked == GL_TRUE;
        }

        if (linked)
        {
            glValidateProgram(handle);
            //Fetch validation info
            {
                GLint isValid = GL_FALSE;
                glGetProgramiv(handle, GL_VALIDATE_STATUS, &isValid);
                GLint maxLength = 0;
                glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &maxLength);
                std::vector<GLchar> info_log = std::vector<GLchar>(maxLength);
                glGetProgramInfoLog(handle, maxLength, &maxLength, &info_log[0]);

                validationLog   = std::string(info_log.begin(), info_log.end());
                valid           = isValid == GL_TRUE;
            }
        }

        //Detatch shaders from program
        for (unsigned int shader : shaders)
        {
            glDetachShader(handle, shader);
        }
    }
    
    //Delete shaders
    for (unsigned int shader : shaders)
    {
        glDeleteShader(shader);
    }

    return valid;
}

void Shader::bind() const
{
    glUseProgram(handle);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform(const std::string& name, int value)
{
    unsigned int location = glGetUniformLocation(handle, name.c_str());
    glUniform1i(location, value);
}

void Shader::setUniform(const std::string& name, float value)
{
    unsigned int location = glGetUniformLocation(handle, name.c_str());
    glUniform1f(location, value);
}

void Shader::setUniform(const std::string& name, const vec2& value)
{
    unsigned int location = glGetUniformLocation(handle, name.c_str());
    glUniform2f(location, value.x, value.y);
}

void Shader::setUniform(const std::string& name, const vec3& value)
{
    unsigned int location = glGetUniformLocation(handle, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::setUniform(const std::string& name, const vec4& value)
{
    unsigned int location = glGetUniformLocation(handle, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::setUniform(const std::string& name, const mat3& value)
{
    unsigned int location = glGetUniformLocation(handle, name.c_str());
    float elements[] = {
        value.m00, value.m01, value.m02,
        value.m10, value.m11, value.m12,
        value.m20, value.m21, value.m22
    };
    glUniformMatrix3fv(location, 1, GL_FALSE, elements);
}

void Shader::setUniform(const std::string& name, const mat4& value)
{
    unsigned int location = glGetUniformLocation(handle, name.c_str());
    float elements[] = {
        value.m00, value.m01, value.m02, value.m03,
        value.m10, value.m11, value.m12, value.m13,
        value.m20, value.m21, value.m22, value.m23,
        value.m30, value.m31, value.m32, value.m33
    };
    glUniformMatrix4fv(location, 1, GL_FALSE, elements);
}