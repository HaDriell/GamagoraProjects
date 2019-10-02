#pragma once

#include "maths.h"
#include <string>

struct Pixel 
{
    vec3 color;
    float depth;
};

struct Framebuffer
{
    unsigned int width;
    unsigned int height;
    Pixel* pixel;

    Framebuffer(unsigned int width = 128, unsigned int height = 128);
    ~Framebuffer();

    void clear();

    void resize(unsigned int width, unsigned int height);

    void write(unsigned int x, unsigned int y, const vec3& color, float depth);

    void save(std::string filename);
};