#pragma once

#include <unordered_map>
#include <string>
#include <memory>

template<typename T>
class AssetManager
{
private:
    std::unordered_map<std::string, std::shared_ptr<T>> assets;

public:
    void clear()
    {
        assets.clear();
    }

    void add(const std::string& name, std::shared_ptr<T> asset)
    {
        assets[name] = asset;
    }

    void remove(const std::string& name)
    {
        assets.erase(name);
    }

    std::shared_ptr<T> find(const std::string& name)
    {
        return assets[name];
    }
};