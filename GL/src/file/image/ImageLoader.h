#pragma once

#include "../../Common.h"
#include "../FileLoader.h"

class ImageLoader : public FileLoader
{
private:
    unsigned int width;
    unsigned int height;
    unsigned char* buffer;

public:
    ImageLoader();

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    const unsigned char* data() const;

protected:
    bool onLoad(const std::string& path);
};