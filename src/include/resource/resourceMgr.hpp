#pragma once

#include <memory>
#include <unordered_map>
#include "font.hpp"
#include "textFile.hpp"
#include "texture.hpp"
#include "../core/common.hpp"
#include "../interface/publisher.hpp"

class ResourceMgr: public ILogMsgPublisher {
public:
    Font*               LoadFont(const ResourceID& resourceID, std::string_view resourcePath);
    void                UnloadFont(const ResourceID& resourceID);
    Font*               GetFont(const ResourceID& resourceID);

    TextFile*           LoadTextFile(const ResourceID& resourceID, std::string_view resourcePath, bool overwrite = false);
    void                UnloadTextFile(const ResourceID& resourceID);
    TextFile*           GetTextFile(const ResourceID& resourceID);

    // Load SimpleTextures
    Texture*            LoadTexture(    const ResourceID& resourceID,
                                        std::string_view resourcePath,
                                        const sf::IntRect pixelRect = sf::IntRect{0, 0, 0, 0});
    // Load RepeatingTextures
    Texture*            LoadTexture(    const ResourceID& resourceID,
                                        std::string_view resourcePath,
                                        Orientation orientation,
                                        unsigned int numRepetitions,
                                        const sf::IntRect pixelRect = sf::IntRect{0, 0, 0, 0});

    Texture*            LoadTexture(    const ResourceID& resourceID,
                                        std::string_view resourcePath,
                                        sf::Vector2u repeatRect,
                                        const sf::IntRect pixelRect = sf::IntRect{0, 0, 0, 0});
    // Load CompositeTextures
    Texture*            LoadTexture(    const ResourceID& resourceID,
                                        const sf::Vector2u& size,
                                        const std::vector<ResourceID>& sourceTextureIDs,
                                        const std::vector<sf::Vector2u>& destinations);

    void                UnloadTexture(const ResourceID& resourceID);
    Texture*            GetTexture(const ResourceID& resourceID);

private:
    std::unordered_map<ResourceID, std::unique_ptr<Font>>               fontMap;
    std::unordered_map<ResourceID, std::unique_ptr<TextFile>>           textFileMap;
    std::unordered_map<ResourceID, std::unique_ptr<Texture>>            textureMap;
};