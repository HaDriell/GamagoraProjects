#include "VFS.h"

#include <fstream>

std::unordered_map<std::string, std::string> VFS::paths;

void VFS::Mount(const std::string& virtualPath, const std::string& physicalPath)
{
    paths[virtualPath] = physicalPath;
}

void VFS::Unmount(const std::string& virtualPath)
{
    paths.erase(virtualPath);
}

bool VFS::Exists(const std::string& physicalPath)
{
    std::fstream file(physicalPath);
    return file.good();
}


bool VFS::Resolve(const std::string& virtualPath, std::string& physicalPath)
{
    //Empty path. Unresolvable.
    if (virtualPath.empty()) 
        return false;

    if (virtualPath[0] == '/')
    {
        unsigned long position = 0;
        while (position < virtualPath.length())
        {
            //Find the next separator
            position = virtualPath.find('/', position);
            //Stop resolving if no more separators are found
            if (position == std::string::npos)
                continue;
            //Extract the Mounting point (smaller mounting points are evaluated first)
            std::string mountingPoint = virtualPath.substr(0, position);
            //Check for existence in mounted paths
            auto vpath = paths.find(mountingPoint);
            if (vpath != paths.end())
            {
                std::string candidatePath = vpath->second + "/" + virtualPath.substr(position + 1);
                if (VFS::Exists(candidatePath))
                {
                    physicalPath = candidatePath;
                    return true;
                }
            }
            position++;
        }
    }

    //No resolution (or not found in VFS)
    physicalPath = virtualPath;
    return VFS::Exists(physicalPath);
}