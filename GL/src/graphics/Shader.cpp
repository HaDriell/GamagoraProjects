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

std::string getGLSLTypeString(GLenum glslType)
{
    switch (glslType)
    {
        case GL_FLOAT:              return "Float";
        case GL_FLOAT_VEC2:         return "Float2";
        case GL_FLOAT_VEC3:         return "Float3";
        case GL_FLOAT_VEC4:         return "Float4";
        case GL_INT:                return "Int";
        case GL_INT_VEC2:           return "Int2";
        case GL_INT_VEC3:           return "Int3";
        case GL_INT_VEC4:           return "Int4";
        case GL_UNSIGNED_INT:       return "UInt";
        case GL_UNSIGNED_INT_VEC2:  return "UInt2";
        case GL_UNSIGNED_INT_VEC3:  return "UInt3";
        case GL_UNSIGNED_INT_VEC4:  return "UInt4";
        case GL_FLOAT_MAT3:         return "Mat3";
        case GL_FLOAT_MAT4:         return "Mat4";
        case GL_SAMPLER_2D:         return "Texture";
    }
    return "Unknown";
}

std::ostream& operator<<(std::ostream& stream, const ShaderUniform& uniform)
{
    stream << "Uniform(" << std::to_string(uniform.index) << ") " << getGLSLTypeString(uniform.type) << " " << uniform.name;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const ShaderAttribute& attribute)
{
    stream << "Attribute(" << std::to_string(attribute.index) << ") " << getGLSLTypeString(attribute.type) << " " << attribute.name;
    return stream;    
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

    if (isCompiled())
    {
        std::cout << "Program is compiled" << std::endl;
    }
    
    if (isCompiled() && isLinked())
    {
        std::cout << "Program is linked" << std::endl;
    }

    if (isCompiled() && isLinked() && isValid())
    {
        std::cout << "Program is valid" << std::endl;
    }

    //Check for compilation errors
    if (!isCompiled())
    {
        std::cout << "Program is NOT compiled" << std::endl;
        for (auto& entry : compilationStatus)
        {
            const std::string& shaderName   = entry.first;
            const std::string& info_log     = compilationLog.at(shaderName);
            std::cout << shaderName << " Shader [" << (entry.second ? "OKAY" : "FAIL") << "]"<< std::endl;
            std::cout << (info_log.empty() ? "None" : info_log) << std::endl;
        }
    }

    //Check for linking errors (only if compilation succeeded)    
    if (isCompiled() && !isLinked())
    {
        std::cout << "Program is NOT linked" << std::endl;
        std::cout << "Linking logs    : " << (linkingLog.empty() ? "None" : linkingLog) << std::endl;
    }

    if (isCompiled() && isLinked() && !isValid())
    {
        std::cout << "Program is NOT valid" << std::endl;
        std::cout << "Validation logs : " << (validationLog.empty() ? "None" : validationLog) << std::endl;
    }

    if (isCompiled() && isLinked() && isValid())
    {
        std::cout << "Shader Uniforms" << std::endl;
        for (auto& entry : shaderUniforms)
        {
            std::cout << '\t' << entry.second << std::endl;
        }

        std::cout << "Shader Attributes" << std::endl;
        for (auto& entry : shaderAttributes)
        {
            std::cout << '\t' << entry.second << std::endl;
        }
    }

    std::cout << "Press Enter to exit Shader debug" << std::endl;
    std::cin.get();
}

bool Shader::compile(const ShaderSources& shaderSources)
{
    //Clear metadata
    compilationLog.clear();
    compilationStatus.clear();
    shaderUniforms.clear();
    shaderAttributes.clear();
    compiled = false;
    linked   = false;
    valid    = false;

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
    compiled = true;
    for (auto& shaderCompilation : compilationStatus)
    {
        if (!shaderCompilation.second)
        {
            compiled = false;
            break;
        }
    }

    //Linking phase
    if (compiled)
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

        //Detatch shaders from program
        for (unsigned int shader : shaders)
        {
            glDetachShader(handle, shader);
        }
    }

    //Validation phase
    if (linked)
    {
        //Validate program
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

    //Query for active uniforms
    if (valid)
    {
        //Fetch Uniforms
        GLint uniformCount;
        glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &uniformCount);
        for (int index = 0; index < uniformCount; index++)
        {
            //Read active uniform data
            GLchar  name[256];
            GLsizei nameLength;
            GLsizei size;
            GLenum  type;
            glGetActiveUniform(handle, index, sizeof(name), &nameLength, &size, &type, name);
            //Store data into the ShaderUniforms
            ShaderUniform shaderUniform;
            shaderUniform.index = index;
            shaderUniform.name  = std::string(name);
            shaderUniform.size  = size;
            shaderUniform.type  = type;
            shaderUniforms[shaderUniform.name] = shaderUniform;
        }

        //Fetch Attributes
        GLint attributeCount;
        glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &attributeCount);
        for (int index = 0; index < attributeCount; index++)
        {
            //Read active attribute data
            GLchar  name[256];
            GLsizei nameLength;
            GLsizei size;
            GLenum  type;
            glGetActiveAttrib(handle, index, sizeof(name), &nameLength, &size, &type, name);
            //Store data into the ShaderUniforms
            ShaderAttribute shaderAttribute;
            shaderAttribute.index = index;
            shaderAttribute.name  = std::string(name);
            shaderAttribute.size  = size;
            shaderAttribute.type  = type;
            shaderAttributes[shaderAttribute.name] = shaderAttribute;
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