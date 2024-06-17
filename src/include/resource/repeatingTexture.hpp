#pragma once

#include "texture.hpp"
#include "../core/common.hpp"

class RepeatingTexture: public Texture {
public:
    RepeatingTexture(       const ResourceID& resID,
                            std::string_view resPath,
                            Orientation orient,
                            unsigned int repeat);

    const sf::Texture&      GetSourceTexture() const;

    Orientation             GetOrientation() const;
    unsigned int            GetNumRepetitions() const;
private:
    sf::Texture             sourceTexture;
    Orientation             orientation;
    unsigned int            numRepetitions;
};