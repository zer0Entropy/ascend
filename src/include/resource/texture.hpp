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

    constexpr static Style  GetStyle(const std::string& name) {
        Style output{Style::Simple};
        if(name.compare("simple") == 0) {
            output = Texture::Style::Simple;
        }
        else if(name.compare("repeating") == 0) {
            output = Texture::Style::Repeating;
        }
        else if(name.compare("composite") == 0) {
            output = Texture::Style::Composite;
        }
        return output;
    }

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