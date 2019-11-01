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

protected:
    bool onLoad(const std::string& path);
};