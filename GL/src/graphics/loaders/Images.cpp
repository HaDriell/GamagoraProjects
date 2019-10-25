#include "Images.h"

#include <FreeImage.h>
#include <iostream>

bool loadImageFile(const std::string& filename, Image& image)
{
    //TODO : group all initializers and put them into an Entry point (or global initializer)
    FreeImage_Initialise();

    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FIBITMAP* bitmap = nullptr;

    //Find the FIF of the file
    fif = FreeImage_GetFileType(filename.c_str());
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
    if (fif == FIF_UNKNOWN)
    {
        std::cout << "Image File loading failed : unsupported format" << std::endl;
        return false;
    }

    if (!FreeImage_FIFSupportsReading(fif))
    {
        std::cout << "Image File loading failed : unreadable format" << std::endl;
        return false;
    }

    bitmap = FreeImage_Load(fif, filename.c_str());
    
    if (!bitmap)
    {
        std::cout << "Image File loading failed : failed to load bitmap" << std::endl;
        return false;
    }

    //OpenGL uses bottom-up textures.
    FreeImage_FlipVertical(bitmap);

    BYTE* pixels = FreeImage_GetBits(bitmap);
    unsigned int width  = FreeImage_GetWidth(bitmap);
    unsigned int height = FreeImage_GetHeight(bitmap);
    unsigned int bitsPerPixel = FreeImage_GetBPP(bitmap);

    bool convertedBitmap = false;
    if (bitsPerPixel == 24 || bitsPerPixel == 32)
    {
        unsigned char* buffer = new unsigned char[width * height * 4];

        //Extract the bitmap
        for (unsigned int y = 0; y < height; y++)
        {
            for (unsigned int x = 0; x < width; x++)
            {
                RGBQUAD color;
                FreeImage_GetPixelColor(bitmap, x, y, &color);
                unsigned int pixel = 4 * (x + y * width);
                //copy color data into an RGBA8 pixel Format
                buffer[pixel + 0] = color.rgbRed;
                buffer[pixel + 1] = color.rgbGreen;
                buffer[pixel + 2] = color.rgbBlue;
                buffer[pixel + 3] = bitsPerPixel == 24 ? 255 : color.rgbReserved;
            }
        }

        image.read(width, height, buffer);
        convertedBitmap = true;
        delete[] buffer;
    }

    //cleanup
    FreeImage_Unload(bitmap);

    return convertedBitmap;
}