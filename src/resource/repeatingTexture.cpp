#include "../include/resource/repeatingTexture.hpp"

RepeatingTexture::RepeatingTexture(     const ResourceID& resID,
                                        std::string_view resPath,
                                        Orientation orient,
                                        unsigned int repeat,
                                        const sf::IntRect sourceRect):
    Texture{resID, resPath, sf::IntRect{0, 0, 0, 0}, Resource::TypeID::RepeatingTexture},
    orientation{orient},
    numRepetitions{repeat} {

    if(sourceRect.width > 0 && sourceRect.height > 0) {
        sourceTexture.loadFromFile(this->GetPath(), sourceRect);
    }
    else {
        sourceTexture.loadFromFile(this->GetPath());
    }
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

RepeatingTexture::RepeatingTexture( const ResourceID& resID,
                                    std::string_view resPath,
                                    const sf::Vector2u& repeatXY,
                                    const sf::IntRect sourceRect):
    Texture{resID, resPath, sourceRect, Resource::TypeID::RepeatingTexture},
    numRepeatHorizontal{repeatXY.x},
    numRepeatVertical{repeatXY.y} {

    if(sourceRect.width > 0 && sourceRect.height > 0) {
        sourceTexture.loadFromFile(this->GetPath(), sourceRect);
    }
    else {
        sourceTexture.loadFromFile(this->GetPath());
    }
    const auto& sourceTextureSize{sourceTexture.getSize()};
    sf::Vector2f destTextureSize{
        (float)(numRepeatHorizontal * sourceTextureSize.x),
        (float)(numRepeatVertical * sourceTextureSize.y)
    };
    sf::Texture& destTexture{this->GetDestTexture()};
    destTexture.create(destTextureSize.x, destTextureSize.y);
    for(int y = 0; y < destTextureSize.y; y += sourceTextureSize.y) {
        for(int x = 0; x < destTextureSize.x; x += sourceTextureSize.x) {
            destTexture.update(sourceTexture, x, y);
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