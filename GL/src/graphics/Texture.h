#pragma once

#include <string>

struct Image
{
private:
    unsigned char* buffer;
    unsigned int width;
    unsigned int height;

public:
    Image(unsigned int width = 1, unsigned int height = 1);
    ~Image();

    void read(unsigned int width, unsigned int height, unsigned char* buffer);

    unsigned int getWidth() const;
    unsigned int getHeight() const;    
    const unsigned char* data() const;
};


enum TextureWrap
{
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
};

enum TextureFilter
{
    NEAREST,
    NEAREST_MIPMAP_LINEAR,
    NEAREST_MIPMAP_NEAREST,
    LINEAR,
    LINEAR_MIPMAP_LINEAR,
    LINEAR_MIPMAP_NEAREST,
};

enum TextureFormat
{
    RGB,
    RGBA,
};

struct TextureSettings
{
    TextureWrap     wrap    = TextureWrap::REPEAT;
    TextureFilter   filter  = TextureFilter::LINEAR;
    TextureFormat   format  = TextureFormat::RGBA;
    bool            mipmap  = false;
};

class Texture
{
private:
    unsigned int        handle;
    //Dimensions
    unsigned int        width;
    unsigned int        height;


    void uploadData(const unsigned char* buffer, unsigned int width, unsigned int height, TextureFormat format);

public:
    Texture();
    ~Texture();

    void defineSettings(const TextureSettings& settings);
    void defineImage(const unsigned char* buffer, unsigned int width, unsigned int height, TextureFormat format);
    void defineImage(const Image& image);
    
    void bind(unsigned int slot = 0) const;
    void unbind(unsigned int slot = 0) const;
};