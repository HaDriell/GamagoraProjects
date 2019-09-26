#pragma once

#include <glm/glm.hpp>
#include <string>

struct Pixel {
    glm::vec3 color;
    float depth;
};

struct Framebuffer
{
    const unsigned int width;
    const unsigned int height;
    Pixel* pixel;

    Framebuffer(unsigned int width, unsigned int height);
    ~Framebuffer();

    void clear();

    void write(unsigned int x, unsigned int y, const glm::vec3& color, float depth);

    void save(std::string filename);
};