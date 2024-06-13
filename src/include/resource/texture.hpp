#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "resource.hpp"

class Texture: public Resource {
public:
    Texture(const ResourceID& resID, std::string_view resPath);

    const sf::Texture&      GetTexture() const;
private:
    sf::Texture             texture;
};