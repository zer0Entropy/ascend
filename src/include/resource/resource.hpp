#pragma once

#include <string>
#include "../core/common.hpp"
#include "../interface/serialize.hpp"

using ResourceID = std::string;

struct ResourceToken {
    ResourceID                  id;
    std::string                 path;
    sf::IntRect                 sourceRect;
    
    ResourceToken(const ResourceID resID, std::string_view resPath):
        id{resID}, path{resPath}, sourceRect{sf::IntRect{0, 0, 0, 0}} { }

    ResourceToken(const ResourceID resID, std::string_view resPath, const sf::IntRect& srcRect):
        id{resID}, path{resPath}, sourceRect{srcRect} { }
};

struct RepeatingTextureToken {
    ResourceID                  id;
    std::string                 path;
    sf::IntRect                 sourceRect;
    Orientation                 orientation;
    unsigned int                numRepetitions;
    bool                        isRect;
    sf::Vector2u                repeatRect;

    RepeatingTextureToken(      const ResourceID& resID,
                                std::string_view resPath,
                                const sf::IntRect& srcRect,
                                Orientation orient,
                                unsigned int numRepeat):
        id{resID},
        path{resPath},
        sourceRect{srcRect},
        orientation{orient},
        numRepetitions{numRepeat},
        isRect{false},
        repeatRect{0, 0} { }

    RepeatingTextureToken(      const ResourceID& resID,
                                std::string_view resPath,
                                const sf::IntRect srcRect,
                                const sf::Vector2u& repRect):
        id{resID},
        path{resPath},
        sourceRect{srcRect},
        isRect{true},
        repeatRect{repRect} { }
};

struct CompositeTextureToken {
    ResourceID                  id;
    sf::Vector2u                size;
    std::vector<ResourceID>     sourceTextureIDs;
    std::vector<sf::Vector2u>   destinations;
};

class Resource {
public:
    enum class TypeID {
        Font,
        SimpleTexture,
        RepeatingTexture,
        CompositeTexture,
        TextFile
    };

    Resource() = delete;
    Resource(const ResourceID& resID, TypeID resType, std::string_view resPath);
    Resource(const Resource& copy) = default;
    Resource(Resource&& move) = default;
    virtual ~Resource() = default;
    Resource& operator=(const Resource& copy) = default;
    Resource& operator=(Resource&& move) = default;

    const ResourceID&       GetID() const;
    TypeID                  GetTypeID() const;
    const std::string&      GetPath() const;

private:
    ResourceID              id;
    TypeID                  typeID;
    std::string             path;
};
