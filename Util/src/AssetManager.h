#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "VFS.h"

template<typename T>
class AssetManager
{
private:
    std::unordered_map<std::string, std::shared_ptr<T>> assets;

public:
    //You must specialize this function for each custom AssetManager you want to use
    static std::shared_ptr<T> LoadAsset(const std::string& path);

public:
    bool load(const std::string& path, const std::string& assetName)
    {
        std::string physicalPath;
        if (VFS::Resolve(path, physicalPath))
        {
            std::shared_ptr<T> asset = AssetManager<T>::LoadAsset(physicalPath);
            if (asset) // check for correct load
            {
                assets.erase(assetName);
                assets[assetName] = asset;
                return true;
            }
        }
        return false;
    }

    void unload(const std::string& assetName)
    {
        assets.erase(assetName);
    }

    std::shared_ptr<T> get(const std::string& assetName)
    {
        return assets[assetName];
    }
};