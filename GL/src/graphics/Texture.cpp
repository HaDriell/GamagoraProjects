#include "Texture.h"

#include <glad/glad.h>
#include <FreeImage.h>
#include <iostream>

GLenum toOpenGLTextureFilter(TextureFilter filter)
{
    switch (filter)
    {
        case TextureFilter::NEAREST:                return GL_NEAREST;
        case TextureFilter::NEAREST_MIPMAP_LINEAR:  return GL_NEAREST_MIPMAP_LINEAR;
        case TextureFilter::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
        case TextureFilter::LINEAR:                 return GL_LINEAR;
        case TextureFilter::LINEAR_MIPMAP_LINEAR:   return GL_LINEAR_MIPMAP_LINEAR;
        case TextureFilter::LINEAR_MIPMAP_NEAREST:  return GL_LINEAR_MIPMAP_NEAREST;
    }
    return GL_FALSE;
}

GLenum toOpenGLTextureWrap(TextureWrap wrap)
{
    switch (wrap)
    {
        case TextureWrap::CLAMP_TO_BORDER:  return GL_CLAMP_TO_BORDER;
        case TextureWrap::CLAMP_TO_EDGE:    return GL_CLAMP_TO_EDGE;
        case TextureWrap::REPEAT:           return GL_REPEAT;
        case TextureWrap::MIRRORED_REPEAT:  return GL_MIRRORED_REPEAT;
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







Image::Image(unsigned int width, unsigned int height) : width(width), height(height), buffer(nullptr)
{
    buffer = new unsigned char[4 * width * height];
}

Image::~Image()
{
    delete[] buffer;
}

void Image::read(unsigned int width, unsigned int height, unsigned char* buffer)
{
    //Deallocate old buffer
    delete[] this->buffer;
    //Update settigns
    this->width     = width;
    this->height    = height;

    //Allocate buffer
    this->buffer = new unsigned char[4 * width * height];
    //Copy buffer
    for (int i = 0; i < 4 * width * height; i++)
    {
        this->buffer[i] = buffer[i];
    }
}

unsigned int Image::getWidth() const
{
    return width;
}

unsigned int Image::getHeight() const
{
    return height;
}

const unsigned char* Image::data() const 
{
    return buffer; 
}









Texture::Texture() : handle(0)
{
    glGenTextures(1, &handle);
}

Texture::~Texture()
{
    glDeleteTextures(1, &handle);
}

void Texture::defineSettings(const TextureSettings& settings)
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
void Texture::defineImage(const Image& image)
{
    uploadData(image.data(), image.getWidth(), image.getHeight(), TextureFormat::RGBA);
}

void Texture::defineImage(const unsigned char* buffer, unsigned int width, unsigned int height, TextureFormat format)
{
    uploadData(buffer, width, height, format);
}

void Texture::uploadData(const unsigned char* buffer, unsigned int width, unsigned int height, TextureFormat format)
{
    GLenum pixelFormat = toOpenGLTextureFormat(format);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, buffer);
}

void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::unbind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}