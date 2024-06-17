#pragma once

#include "texture.hpp"
#include "../core/common.hpp"

class RepeatingTexture: public Texture {
public:
    RepeatingTexture(       const ResourceID& resID,
                            std::string_view resPath,
                            Orientation orient,
                            unsigned int repeat,
                            const sf::IntRect sourceRect);

    RepeatingTexture(       const ResourceID& resID,
                            std::string_view resPath,
                            const sf::Vector2u& repeatXY,
                            const sf::IntRect sourceRect);

    const sf::Texture&      GetSourceTexture() const;

    Orientation             GetOrientation() const;
    unsigned int            GetNumRepetitions() const;
private:
    sf::Texture             sourceTexture;
    Orientation             orientation;
    unsigned int            numRepetitions;
    unsigned int            numRepeatHorizontal;
    unsigned int            numRepeatVertical;
};