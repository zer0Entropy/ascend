#include "../include/resource/compositeTexture.hpp"

CompositeTexture::CompositeTexture(ResourceID resID, const sf::Vector2u size):
    Texture{resID, std::string{""}, Resource::TypeID::CompositeTexture} {
    sf::Texture&            destTexture{this->GetDestTexture()};
    destTexture.create(size.x, size.y);
}

void CompositeTexture::AddTexture(const sf::Texture& srcTexture, sf::Vector2u destination) {
    sf::Texture&            destTexture{this->GetDestTexture()};
    destTexture.update(srcTexture, destination.x, destination.y);
}
