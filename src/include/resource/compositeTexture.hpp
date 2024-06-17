#pragma once

#include "texture.hpp"

class CompositeTexture: public Texture {
public:
    CompositeTexture(ResourceID resID, const sf::Vector2u size);

    void                            AddTexture(const sf::Texture& srcTexture, sf::Vector2u destination);
};