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
};

template<typename T>
AssetManager<T>* Assets<T>::instance = nullptr;