#include "../include/resource/texture.hpp"

Texture::Texture(const ResourceID& resID, std::string_view resPath):
    Resource{resID, Resource::TypeID::Texture, resPath} {
    texture.loadFromFile(this->GetPath());
}

const sf::Texture& Texture::GetTexture() const {
    return texture;
}