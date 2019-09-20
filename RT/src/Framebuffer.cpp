#include "Framebuffer.h"
#include "Util.h"
#include <FreeImage.h>

void Framebuffer::save(std::string filename)
{
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);
    RGBQUAD color;    
    for(int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            glm::vec3& pixel = data[i + j * width];
            color.rgbRed        = lerp(0, 255, clamp(0, 1, pixel.x));
            color.rgbGreen      = lerp(0, 255, clamp(0, 1, pixel.y));
            color.rgbBlue       = lerp(0, 255, clamp(0, 1, pixel.z));
            color.rgbReserved   = 255;
            FreeImage_SetPixelColor(bitmap, i, j, &color);
        }
    }
    FreeImage_Save(FIF_PNG, bitmap, filename.c_str());
//    FreeImage_DeInitialise();
}

void Framebuffer::set(unsigned int x, unsigned int y, const glm::vec3& color)
{
    if (x >= width) return;
    if (y >= height) return;
    data[x + y * width] = color;
}