#pragma once

#include <SFML/Graphics/Texture.hpp>
#include "resource.hpp"

class Texture: public Resource {
public:
    enum class Style {
        Simple,
        Repeating,
        Composite
    };

    Texture() = delete;
    Texture(                const ResourceID& resID,
                            std::string_view resPath,
                            const sf::IntRect sourceRect = sf::IntRect{0, 0, 0, 0},
                            Resource::TypeID resType = Resource::TypeID::SimpleTexture);
    Texture(const Texture& copy) = default;
    Texture(Texture&& move) = default;
    virtual ~Texture() = default;
    Texture& operator=(const Texture& copy) = default;
    Texture& operator=(Texture&& move) = default;

    const sf::Texture&      GetTexture() const;

    Style                   GetStyle() const;
    
protected:
    sf::Texture&            GetDestTexture() const;

private:
    sf::Texture             texture;
    Style                   style;
};