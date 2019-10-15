#pragma once

#include "maths.h"
#include <string>
#include <map>

using ShaderSources = std::map<unsigned int, std::string>;

class Shader
{
private:
    int handle;

public:
    Shader();
    ~Shader();

    void compileFile(const std::string& path);    
    void compileSources(const ShaderSources& sources);
    void destroy();
    
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