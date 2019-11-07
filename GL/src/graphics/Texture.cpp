#include "Texture.h"

#include <glad/glad.h>
#include "../File.h"


////////////////////////////////////////////////////////////////////////////////
// OpenGL enum Converters //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

GLenum toOpenGLTextureFilter(TextureFilter filter)
{
    switch (filter)
    {
        case TextureFilter::Nearest:                return GL_NEAREST;
        case TextureFilter::NearestMipmapLinear:    return GL_NEAREST_MIPMAP_LINEAR;
        case TextureFilter::NearestMipmapNearest:   return GL_NEAREST_MIPMAP_NEAREST;
        case TextureFilter::Linear:                 return GL_LINEAR;
        case TextureFilter::LinearMipmapLinear:     return GL_LINEAR_MIPMAP_LINEAR;
        case TextureFilter::LinearMipmapNearest:    return GL_LINEAR_MIPMAP_NEAREST;
    }
    return GL_FALSE;
}

GLenum toOpenGLTextureWrap(TextureWrap wrap)
{
    switch (wrap)
    {
        case TextureWrap::ClampToBorder:    return GL_CLAMP_TO_BORDER;
        case TextureWrap::ClampToEdge:      return GL_CLAMP_TO_EDGE;
        case TextureWrap::Repeat:           return GL_REPEAT;
        case TextureWrap::MirroredRepeat:   return GL_MIRRORED_REPEAT;
    }
    return GL_FALSE;
}

GLenum toOpenGLTextureFormat(TextureFormat format)
{
    switch (format)
    {
        case TextureFormat::RGB:    return GL_RGB;
        case TextureFormat::RGBA:   return GL_RGBA;
    }
    return GL_FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Texture2D Functions /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Texture2D::Texture2D() : handle(0)
{
    glGenTextures(1, &handle);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &handle);
}


void Texture2D::loadImage(const std::string& path, const TextureSettings& settings)
{
    ImageLoader loader;
    if(loader.load(path))
    {
        defineImage(loader.data(), loader.getWidth(), loader.getHeight(), TextureFormat::RGBA);
        defineSettings(settings);
        LogDebug("Texture2D loaded from Image file '{0}'.  Dimensions {1}x{2}.", path, loader.getWidth(), loader.getHeight());
    } 
    else
    {
        LogWarning("Failed to resolve Image file '{0}'.", path);
    } 
}

void Texture2D::defineSettings(const TextureSettings& settings)
{
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toOpenGLTextureFilter(settings.filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toOpenGLTextureFilter(settings.filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toOpenGLTextureWrap(settings.wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toOpenGLTextureWrap(settings.wrap));
    if (settings.mipmap)
    {
        //TODO : check how mipmap is actually handled, this might be totally wrong here
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Texture2D::defineImage(const unsigned char* buffer, unsigned int width, unsigned int height, TextureFormat format)
{
    uploadData(buffer, width, height, format);
}

void Texture2D::uploadData(const unsigned char* buffer, unsigned int width, unsigned int height, TextureFormat format)
{
    GLenum pixelFormat = toOpenGLTextureFormat(format);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, buffer);
}

void Texture2D::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture2D::unbind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}