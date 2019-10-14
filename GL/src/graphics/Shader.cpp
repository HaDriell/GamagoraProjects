#include "Shader.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <glad/glad.h>

int __compile_shader(unsigned int shaderType, const std::string& source);
int __compile_program(const ShaderSources& sources);


Shader::Shader() : handle(0) {}

Shader::~Shader()
{
    destroy();
}

void Shader::compileFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "Failed to compile Shader : File '" << path << "' not found !" << std::endl;
        return;
    }
    //parse file
    ShaderSources sources;

    unsigned int shaderSection = GL_FALSE;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.rfind("//Vertex Shader", 0) == 0)
        {
            shaderSection = GL_VERTEX_SHADER;
        }
        else if (line.rfind("//Fragment Shader", 0) == 0)
        {
            shaderSection = GL_FRAGMENT_SHADER;
        }
        else if (shaderSection)
        {
            sources[shaderSection] += line + "\n";
        }
    }

    compileSources(sources);
}

void Shader::compileSources(const ShaderSources& sources)
{
    handle = __compile_program(sources);
}


void Shader::destroy()
{
    if (handle)
    {
        glDeleteProgram(handle);
        handle = 0;
    }
}

void Shader::bind()
{
    glUseProgram(handle);
}
void Shader::unbind()
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

int __compile_shader(unsigned int shaderType, const std::string& source)
{
    int shader = glCreateShader(shaderType);
    const GLchar* src = source.c_str();
    glShaderSource(shader, 1, &src, 0);
    glCompileShader(shader);

    GLint isCompiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> info_log = std::vector<GLchar>(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &info_log[0]);
        glDeleteShader(shader);
        shader = 0;

        std::cout << "Failed to compile shader : " << info_log.data() << std::endl;
    }
    return shader;
}


int __compile_program(const ShaderSources& sources)
{
    int program = 0;

    //Compile Shaders
    std::vector<int> shaders;
    for (auto& kv : sources)
    {
        int shader = __compile_shader(kv.first, kv.second);

        if (shader)
            shaders.push_back(shader);
        else
            break;
    }

    //Link program step if each shader compiled successfuly
    if (shaders.size() == sources.size())
    {
        program = glCreateProgram();
        for (int shader : shaders)
            glAttachShader(program, shader);
        glLinkProgram(program);

        //Check for program link status
        GLint isLinked = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked != GL_TRUE) 
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> info_log = std::vector<GLchar>(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &info_log[0]);
            std::cout << "Shader program linking failed : " << info_log.data() << std::endl;

            //Clean program attachments
            for (int shader : shaders)
            {
                glDetachShader(program, shader);
            }
            //Delete program
            glDeleteProgram(program);
            program = 0;
        }

        //Don't care about validation for now
    }

    //Clean-Up Shaders in any case
    for (int shader : shaders)
    {
        glDeleteShader(shader);
    }

    //Return program
    return program;
}