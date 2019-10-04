#include "Resources.h"

sf::Texture& Resources::getTexture(const std::string& path)
{
    //Get from cache
    auto cache = textures.find(path);
    if (cache != textures.end())
        return cache->second;
    
    //Load into cache
    auto& texture = textures[path];
    texture.loadFromFile(path);
    return texture;
}