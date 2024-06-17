#include "../include/resource/texture.hpp"

Texture::Texture(           const ResourceID& resID,
                            std::string_view resPath,
                            Resource::TypeID resType):
    Resource{resID, resType, resPath} {
    
    if(resType == Resource::TypeID::SimpleTexture) {
        style = Style::Simple;
        texture.loadFromFile(this->GetPath());
    }
    else if(resType == Resource::TypeID::RepeatingTexture) {
        style = Style::Repeating;
    }
    else if(resType == Resource::TypeID::CompositeTexture) {
        style = Style::Composite;
    }
}

const sf::Texture& Texture::GetTexture() const {
    return texture;
}

sf::Texture& Texture::GetDestTexture() const {
    return const_cast<sf::Texture&>(texture);
}

Texture::Style Texture::GetStyle() const {
    return style;
}