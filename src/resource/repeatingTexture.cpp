#include "../include/resource/repeatingTexture.hpp"

RepeatingTexture::RepeatingTexture(     const ResourceID& resID,
                                        std::string_view resPath,
                                        Orientation orient,
                                        unsigned int repeat):
    Texture{resID, resPath, Resource::TypeID::RepeatingTexture},
    orientation{orient},
    numRepetitions{repeat} {

    sourceTexture.loadFromFile(this->GetPath());
    const auto& sourceTextureSize{sourceTexture.getSize()};
    sf::Vector2f destTextureSize{0.0f, 0.0f};
    if(orientation == Orientation::Horizontal) {
        destTextureSize.x = sourceTextureSize.x * numRepetitions;
        destTextureSize.y = sourceTextureSize.y;
    }
    else if(orientation == Orientation::Vertical) {
        destTextureSize.x = sourceTextureSize.x;
        destTextureSize.y = sourceTextureSize.y * numRepetitions;
    }
    sf::Texture& destTexture{this->GetDestTexture()};
    destTexture.create(destTextureSize.x, destTextureSize.y);

    if(orientation == Orientation::Horizontal) {
        for(int n = 0; n < numRepetitions; ++n) {
            int x = n * sourceTextureSize.x;
            destTexture.update(sourceTexture, x, 0);
        }
    }
    else if(orientation == Orientation::Vertical) {
        for(int n = 0; n < numRepetitions; ++n) {
            int y = n * sourceTextureSize.y;
            destTexture.update(sourceTexture, 0, y);
        }
    }
}

const sf::Texture& RepeatingTexture::GetSourceTexture() const {
    return sourceTexture;
}

Orientation RepeatingTexture::GetOrientation() const {
    return orientation;
}

unsigned int RepeatingTexture::GetNumRepetitions() const {
    return numRepetitions;
}