#include "FileLoader.h"


bool FileLoader::load(const std::string& path)
{
    std::string physicalPath;
    if (!VFS::Resolve(path, physicalPath))
        return false;
    return onLoad(physicalPath);
}