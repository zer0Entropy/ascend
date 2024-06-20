#pragma once

#include "resource.hpp"

struct ResourceToken {
    ResourceID                      id;
    std::string                     path;
    Resource::TypeID                typeID;

    struct TextureParams {
        sf::IntRect                 sourceRect;
        unsigned int                repeatHorizontal;
        unsigned int                repeatVertical;
        sf::Vector2u                size;
        std::vector<ResourceID>     sourceTextureIDs;
        std::vector<sf::Vector2u>   destinations;

        TextureParams():
            sourceRect{0, 0, 0, 0},
            repeatHorizontal{0},
            repeatVertical{0},
            size{0, 0} {

        }

        TextureParams(const TextureParams& copy) = default;
        TextureParams(TextureParams&& move) = default;
        TextureParams& operator=(const TextureParams& copy) = default;
        TextureParams& operator=(TextureParams&& move) = default;

        TextureParams(const sf::IntRect& srcRect):
            sourceRect{srcRect},
            repeatHorizontal{0},
            repeatVertical{0},
            size{0, 0} {

        }

        TextureParams(  unsigned int horizontal,
                        unsigned int vertical,
                        const sf::IntRect& srcRect = sf::IntRect{0, 0, 0, 0}):
            sourceRect{srcRect},
            repeatHorizontal{horizontal},
            repeatVertical{vertical},
            size{0, 0} {

        }

        TextureParams(  const sf::Vector2u& textureSize,
                        const std::vector<ResourceID>& sources,
                        const std::vector<sf::Vector2u>& dests):
            sourceRect{0, 0, 0, 0},
            repeatHorizontal{0},
            repeatVertical{0},
            size{textureSize},
            sourceTextureIDs{sources},
            destinations{dests} {

        }
    };

    TextureParams                   textureParams;
    
    ResourceToken(const ResourceID resID, std::string_view resPath, Resource::TypeID resType):
        id{resID}, path{resPath}, typeID{resType} { }

    ResourceToken(  const ResourceID resID,
                    std::string_view resPath,
                    Resource::TypeID resType,
                    const TextureParams& txtParams):
        id{resID}, path{resPath}, typeID{resType}, textureParams{txtParams} { }
};