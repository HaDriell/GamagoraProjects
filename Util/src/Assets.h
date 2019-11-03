#pragma once

#include "AssetManager.h"

template<typename T>
struct Assets
{
private:
    static AssetManager<T>* instance;

public:
    static AssetManager<T>* Get()
    {
        if (instance == nullptr)
            instance = new AssetManager<T>();
        return instance;
    }

    inline static void Clear()
    {
        Get()->clear();
    }

    inline static void Add(const std::string& name, std::shared_ptr<T> asset)
    {
        Get()->add(name, asset);
    }

    inline static void Remove(const std::string& name)
    {
        Get()->remove(name);
    }

    inline static std::shared_ptr<T> Find(const std::string& name)
    {
        return Get()->find(name);
    }
};

template<typename T>
AssetManager<T>* Assets<T>::instance = nullptr;