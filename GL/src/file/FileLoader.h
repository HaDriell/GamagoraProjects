#pragma once

#include <VFS.h>

class FileLoader
{
public:
    bool load(const std::string& path);

protected:
    virtual bool onLoad(const std::string& path) = 0;
};