#include "Framebuffer.h"

#include <glad/glad.h>
#include <iostream>

Framebuffer::Framebuffer(unsigned int width, unsigned int height) : handle(0), width(width), height(height)
{
    //Color Buffer initialization
    {
        TextureSettings settings;
        settings.filter = TextureFilter::Linear;
        settings.wrap   = TextureWrap::ClampToEdge;
        
        colorTexture = MakeRef<Texture2D>();
        colorTexture->bind();
        colorTexture->defineSettings(settings);
        colorTexture->defineEmptyImage(TextureInternalFormat::RGBA8, width, height);
    }

    std::cout << "\n\n\n\n\n\n\n" << std::endl;
    
    //Depth Buffer initialization
    {
        TextureSettings settings;
        settings.filter = TextureFilter::Linear;
        settings.wrap   = TextureWrap::ClampToEdge;

        depthTexture = MakeRef<Texture2D>();
        depthTexture->defineSettings(settings);
        depthTexture->defineEmptyImage(TextureInternalFormat::DEPTH_COMPONENT32F, width, height);
    }

    //Framebuffer initialization
    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->handle, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, depthTexture->handle, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &handle);
}

unsigned int Framebuffer::getWidth() const
{
    return width;
}

unsigned int Framebuffer::getHeight() const
{
    return height;
}

Ref<Texture2D> Framebuffer::getColorBuffer() const
{
    return colorTexture;
}

Ref<Texture2D> Framebuffer::getDepthBuffer() const
{
    return depthTexture;
}

bool Framebuffer::isComplete() const
{
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::debug() const
{
    switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
    case GL_FRAMEBUFFER_COMPLETE:
        LogInfo("Framebuffer is complete");
    break;

    case GL_FRAMEBUFFER_UNDEFINED:
        LogError("Framebuffer doesn't exist");
    break;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        LogError("Framebuffer contains at least one attachement that is incomplete");
    break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        LogError("Framebuffer has no attached Textures");
    break;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        LogError("Framebuffer draw target is incomplete");
    break;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        LogError("Framebuffer read target is incomplete");
    break;

    default:
        LogError("Framebuffer is unsupported");
    break;
    }

    std::cout << "Press any key to Continue..." << std::endl;
    std::cin.get();
}

void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}