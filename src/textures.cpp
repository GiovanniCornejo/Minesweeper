#include "textures.h"

std::unordered_map<std::string, std::shared_ptr<sf::Texture>> Textures::textures;

std::shared_ptr<sf::Texture> Textures::getTexture(const std::string &name)
{
    if (textures.find(name) == textures.end())
        loadTexture(name);

    return Textures::textures[name];
}

void Textures::loadTexture(const std::string &file)
{
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(file))
        throw std::runtime_error("ERROR: Failed to load texture from file: " + file);

    textures[file] = texture;
}