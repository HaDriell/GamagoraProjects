#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>


class Resources
{
private:
    std::map<std::string, sf::Texture> textures;

public:
    sf::Texture& getTexture(const std::string& path);
};
