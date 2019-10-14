#pragma once

#include <string>
#include <glad/glad.h>

enum TextureWrap
{
    REPEAT          = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE
};

enum TextureFilter
{
    LINEAR  = GL_LINEAR,
    NEAREST = GL_NEAREST
};

enum TextureFormat
{
    RGB     = GL_RGB,
    RGBA    = GL_RGBA
};

struct TextureSettings
{
    TextureWrap     wrap    = TextureWrap::REPEAT;
    TextureFilter   filter  = TextureFilter::LINEAR;
    TextureFormat   format  = TextureFormat::RGBA;
};

class Texture
{
private:
    unsigned int        handle;
    unsigned int        width;
    unsigned int        height;
    TextureSettings     settings;

public:
    Texture();
    ~Texture();

    void destroy();
    void loadFile(const std::string& path);

    TextureSettings getSettings();
    void setSettings(const TextureSettings& settings);
    
    void bind(unsigned int slot = 0) const;
    void unbind(unsigned int slot = 0) const;
};