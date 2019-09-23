#include "Framebuffer.h"
#include "Util.h"
#include <FreeImage.h>


Framebuffer::Framebuffer(unsigned int width, unsigned int height) : width(width), height(height) 
{
    pixel = new Pixel[width * height];
    clear();
}

Framebuffer::~Framebuffer()
{
    delete[] pixel;
}


void Framebuffer::save(std::string filename)
{
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);
    RGBQUAD color;    
    for(int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            glm::vec3& p = pixel[i + j * width].color;
            color.rgbRed        = lerp(0, 255, clamp(0, 1, p.x));
            color.rgbGreen      = lerp(0, 255, clamp(0, 1, p.y));
            color.rgbBlue       = lerp(0, 255, clamp(0, 1, p.z));
            color.rgbReserved   = 255;
            FreeImage_SetPixelColor(bitmap, i, j, &color);
        }
    }
    FreeImage_Save(FIF_PNG, bitmap, filename.c_str());
//    FreeImage_DeInitialise();
}



void Framebuffer::clear()
{
    for (int i = 0; i < width * height; i++)
    {
        pixel[i].color = glm::vec3{0.f, 0.f, 0.f};
        pixel[i].depth = std::numeric_limits<float>::max();
    }
}

void Framebuffer::write(unsigned int x, unsigned int y, const glm::vec3& color, float depth)
{
    if (x >= width) return;
    if (y >= height) return;
    if (depth > pixel[x + y * width].depth) return;
    pixel[x + y * width].color = color;
    pixel[x + y * width].depth = depth;
}