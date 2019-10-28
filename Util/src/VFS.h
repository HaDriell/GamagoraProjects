#pragma once

#include <unordered_map>
#include <string>

class VFS
{
private:
    static std::unordered_map<std::string, std::string> paths;

public:
    static void Mount(const std::string& virtualPath, const std::string& physicalPath);
    static void Unmount(const std::string& virtualPath);
    static bool Exists(const std::string& physicalPath);
    static bool Resolve(const std::string& virtualPath, std::string& physicalPath);
};