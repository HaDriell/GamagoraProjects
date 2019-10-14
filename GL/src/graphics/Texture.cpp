#include "Texture.h"

#include <glad/glad.h>
#include <FreeImage.h>
#include <iostream>


Texture::Texture() : handle(0) {}

Texture::~Texture()
{
    destroy();
}

TextureSettings Texture::getSettings()
{
    return settings;
}

void Texture::setSettings(const TextureSettings& settings)
{
    this->settings = settings;
    //Update OpenGL if Texture exists
    if (handle)
    {
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.wrap);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::destroy()
{
    if (handle)
    {
        glDeleteTextures(1, &handle);
        handle = 0;
    }
}

void Texture::loadFile(const std::string& path)
{
    FreeImage_Initialise(); // Spam initialization... what else can I do ?

    if (!handle)
    {
        glGenTextures(1, &handle);
    }

    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FIBITMAP* bitmap = nullptr;

    //Find the FIF of the file
    fif = FreeImage_GetFileType(path.c_str());
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(path.c_str());
    if (fif == FIF_UNKNOWN)
    {
        std::cout << "Texture loading failed : unsupported format" << std::endl;
        return;
    }

    if (!FreeImage_FIFSupportsReading(fif))
    {
        std::cout << "Texture loading failed : unreadable format" << std::endl;
        return;
    }

    bitmap = FreeImage_Load(fif, path.c_str());
    
    if (!bitmap)
    {
        std::cout << "Texture loading failed : failed to load bitmap" << std::endl;
        return;
    }

    //Apparently FreeImage loads from bottom to top ?
    FreeImage_FlipVertical(bitmap);

    BYTE* pixels = FreeImage_GetBits(bitmap);
    width  = FreeImage_GetWidth(bitmap);
    height = FreeImage_GetHeight(bitmap);

    //Update the pixel format
    unsigned int bpp = FreeImage_GetBPP(bitmap);
    switch (bpp)
    {
        case 24:
            settings.format = TextureFormat::RGB;
        break;

        case 32:
            settings.format = TextureFormat::RGBA;        
        break;
        
        default:
            std::cout << "Unsupported BPP Format : " << bpp << std::endl;
            return;
    }

    //Upload image data
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.wrap);
    glTexImage2D(GL_TEXTURE_2D, 0, settings.format, width, height, 0, settings.format, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    FreeImage_Unload(bitmap);
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