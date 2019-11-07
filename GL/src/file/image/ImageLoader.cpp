#include "ImageLoader.h"
#include <FreeImage.h>

ImageLoader::ImageLoader() : buffer(nullptr), width(0), height(0)
{
}


bool ImageLoader::onLoad(const std::string& path)
{
    //Check for freeimage initialization
    static bool freeimageInitialized = false;
    if (!freeimageInitialized)
    {
        FreeImage_Initialise();
        freeimageInitialized = true;
    }
    
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FIBITMAP* bitmap = nullptr;

    //Find the FIF of the file
    fif = FreeImage_GetFileType(path.c_str());
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(path.c_str());
    if (fif == FIF_UNKNOWN)
    {
        LogError("Failed to load Image File '{0}' : Unkown format", path);
        return false;
    }

    if (!FreeImage_FIFSupportsReading(fif))
    {
        LogError("Failed to load Image File '{0}' : Unsupported format", path);
        return false;
    }

    bitmap = FreeImage_Load(fif, path.c_str());
    
    if (!bitmap)
    {
        LogError("Failed to load Image File '{0}' : Failed to load bitmap", path);
        return false;
    }

    //OpenGL uses bottom-up textures.
    FreeImage_FlipVertical(bitmap);

    BYTE* pixels = FreeImage_GetBits(bitmap);
    width  = FreeImage_GetWidth(bitmap);
    height = FreeImage_GetHeight(bitmap);
    //Clean-up previous buffer
    if (buffer)
    {
        delete[] buffer;
    }
    buffer = new unsigned char[width * height * 4];

    unsigned int bitsPerPixel = FreeImage_GetBPP(bitmap);

    bool loadedImage = false;
    if (bitsPerPixel == 24 || bitsPerPixel == 32)
    {
        //Extract the bitmap
        for (unsigned int y = 0; y < height; y++)
        {
            for (unsigned int x = 0; x < width; x++)
            {
                RGBQUAD color;
                FreeImage_GetPixelColor(bitmap, x, y, &color);
                unsigned int pixel = 4 * (x + y * width);

                //copy color data into an RGBA8 pixel Format
                this->buffer[pixel + 0] = color.rgbRed;
                this->buffer[pixel + 1] = color.rgbGreen;
                this->buffer[pixel + 2] = color.rgbBlue;
                this->buffer[pixel + 3] = bitsPerPixel == 24 ? 255 : color.rgbReserved;
            }
        }
        loadedImage = true;
    }

    FreeImage_Unload(bitmap);
    return loadedImage;
}

unsigned int ImageLoader::getWidth() const
{
    return width;
}

unsigned int ImageLoader::getHeight() const
{
    return height;
}

const unsigned char* ImageLoader::data() const
{
    return buffer;
}