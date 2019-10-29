#pragma once

#include <memory>
#include <string>
#include <map>

#include "maths.h"

//Represents a single uniform in the program
struct ShaderUniform
{
    std::string     name;
    unsigned int    index;
    unsigned int    size;
    unsigned int    type;
};
std::ostream& operator<<(std::ostream& stream, const ShaderUniform& uniform);

//Represents a single attribute in the vertex shader
struct ShaderAttribute
{
    std::string     name;
    unsigned int    index;
    unsigned int    size;
    unsigned int    type;
};
std::ostream& operator<<(std::ostream& stream, const ShaderAttribute& attribute);

using ShaderSources         = std::map<unsigned int, std::string>;
using ShaderCompilationLogs = std::map<std::string, std::string>;
using ShaderCompileStatuses = std::map<std::string, bool>;
using ShaderUniforms        = std::map<std::string, ShaderUniform>;
using ShaderAttributes      = std::map<std::string, ShaderAttribute>;

class Shader
{
private:
    int handle;

    //Useful metadata for development
    ShaderCompilationLogs   compilationLog;
    ShaderCompileStatuses   compilationStatus;
    ShaderUniforms          shaderUniforms;
    ShaderAttributes        shaderAttributes;
    bool                    compiled;
    std::string             linkingLog;
    bool                    linked;
    std::string             validationLog;
    bool                    valid;

public:
    Shader();
    ~Shader();

    void debug() const;
    bool compile(const ShaderSources& sources);

    //metadata Getters
    bool isCompiled() const { return compiled; }
    bool isValid() const { return valid; }
    bool isLinked() const { return linked; }
    const ShaderCompilationLogs& getCompilationLog() const { return compilationLog; }
    const ShaderCompileStatuses& getCompilationStatus() const { return compilationStatus; }
    const ShaderUniforms& getShaderUniforms() const { return shaderUniforms; }
    const std::string& getLinkingLog() const { return linkingLog; }
    const std::string& getValidationLog() const { return validationLog; }

    void bind() const;
    void unbind() const;

    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const vec2& value);
    void setUniform(const std::string& name, const vec3& value);
    void setUniform(const std::string& name, const vec4& value);
    void setUniform(const std::string& name, const mat3& value);
    void setUniform(const std::string& name, const mat4& value);
};