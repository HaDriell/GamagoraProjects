#pragma once

#include <glm/glm.hpp>
#include <string>

struct Framebuffer
{
    const unsigned int width;
    const unsigned int height;
    glm::vec3* data;

    Framebuffer(unsigned int width, unsigned int height) : width(width), height(height) 
    {
        data = new glm::vec3[width * height];
    }

    ~Framebuffer()
    {
        delete[] data;
    }

    void set(unsigned int x, unsigned int y, const glm::vec3& color);

    void save(std::string filename);
};