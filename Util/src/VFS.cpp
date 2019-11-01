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
    //Empty path. Always fail.
    if (virtualPath.empty()) 
        return false;

    if (virtualPath[0] == '/')
    {
        unsigned long position = 0;
        //While finding positions in virtualPath (always will be less than npos)
        while ((position = virtualPath.find('/', position)) < virtualPath.length())
        {
            std::string prefix = virtualPath.substr(0, position);
            //Check for existence in mounted paths
            auto vpath = paths.find(prefix);
            if (vpath != paths.end())
            {
                //Assemble an actual path & test if exists
                std::string candidatePath = vpath->second + "/" + virtualPath.substr(position + 1);
                if (VFS::Exists(candidatePath))
                {
                    physicalPath = candidatePath;
                    return true;
                }
            }
            //Advance once to find the next separator
            position++;
        }
    }

    //No resolution (or not found in VFS)
    physicalPath = virtualPath;
    return VFS::Exists(physicalPath);
}