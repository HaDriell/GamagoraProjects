#pragma once

#include "../Common.h"

#include "Texture.h"


class Framebuffer
{
private:
    unsigned int handle;
    unsigned int width;
    unsigned int height;

    Ref<Texture2D>  colorTexture;
    Ref<Texture2D>  depthTexture;

public:
    Framebuffer(unsigned int width, unsigned int height);
    ~Framebuffer();

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    Ref<Texture2D> getColorBuffer() const;
    Ref<Texture2D> getDepthBuffer() const;

    bool isComplete() const;
    void debug() const;

    void bind();
    void unbind();
};