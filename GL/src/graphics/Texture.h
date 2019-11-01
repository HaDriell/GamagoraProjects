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
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,
};

enum TextureFilter
{
    Nearest,
    NearestMipmapLinear,
    NearestMipmapNearest,
    Linear,
    LinearMipmapLinear,
    LinearMipmapNearest,
};

enum TextureFormat
{
    RGB,
    RGBA,
};

struct TextureSettings
{
    TextureWrap     wrap    = TextureWrap::Repeat;
    TextureFilter   filter  = TextureFilter::Linear;
    TextureFormat   format  = TextureFormat::RGBA;
    bool            mipmap  = false;
};

class Texture2D
{
private:
    unsigned int        handle;
    //Dimensions
    unsigned int        width;
    unsigned int        height;


    void uploadData(const unsigned char* buffer, unsigned int width, unsigned int height, TextureFormat format);

public:
    Texture2D();
    ~Texture2D();

    void defineSettings(const TextureSettings& settings);
    void defineImage(const unsigned char* buffer, unsigned int width, unsigned int height, TextureFormat format);
    void defineImage(const Image& image);
    
    void bind(unsigned int slot = 0) const;
    void unbind(unsigned int slot = 0) const;
};