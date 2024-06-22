#pragma once

#include "layer.hpp"
#include "../resource/token.hpp"

class SceneParser {
public:
    SceneParser(const nlohmann::json& json);

    // Returns a list containing all layers' names
    std::vector<std::string>                            LoadLayerIndex();

    // Returns a list containing all layers
    std::vector<Layer>                                  LoadLayers();

    // Returns the TypeID of Layer stored at layerJSONList[index]
    Layer::TypeID                                       LoadLayerType(int index);

    // Returns a list containing one ResourceToken for each Resource in layerJSONList[index]
    std::vector<ResourceToken>                          LoadResources(int index);

    // Return a list containing one ResourceToken for each Resource (of the type specified) in json parameter
    std::vector<ResourceToken>                          LoadFonts(const nlohmann::json& json) const;
    std::vector<ResourceToken>                          LoadTextures(const nlohmann::json& json) const;
    std::vector<ResourceToken>                          LoadMusics(const nlohmann::json& json) const;
    std::vector<ResourceToken>                          LoadSounds(const nlohmann::json& json) const;

    // Returns a list containing a sf::Rect<uint> for each BoundingBox in layerJSONList[index]
    std::vector<sf::Rect<unsigned int>>                 LoadBoundingBoxes(int index) const;

    // Returns a list containing a ResourceID for each Sprite in layerJSONList[index]
    std::vector<ResourceID>                             LoadSpriteAttachments(int index) const;

    // Returns a list containing a ResourceID for each Text in layerJSONList[index]
    std::vector<ResourceID>                             LoadTextAttachments(int index) const;

    // Returns a list containing a std::string for each Text in layerJSONList[index]
    std::vector<std::string>                            LoadTextContents(int index) const;

    // Returns a list containing a FontParam for each Text in layerJSONList[index]
    std::vector<Text::FontParameters>                   LoadFontParam(int index) const;

    // Returns a ResourceID for BackgroundMusic in layerJSONList[index]
    ResourceID                                          LoadBackgroundMusic(int index) const;

    // Returns PlaybackParameters for BackgroundMusic in layerJSONList[index]
    Music::PlaybackParam                                LoadPlaybackParam(int index) const;

    // Returns a list containing an Alignment for each Alignable in layerJSONList[index]
    std::vector<Alignment>                              LoadAlignments(int index) const;

    // Returns a list containing a pair<Event::TypeID, ResourceID> for each TextureSwitch in layerJSONList[index]
    std::vector<std::pair<Event::TypeID, ResourceID>>   LoadTextureSwitches(int index) const;

    // Returns a list containing a pair<Event::TypeID, ResourceID> for each SoundEffect in layerJSONList[index]
    std::vector<std::pair<Event::TypeID, ResourceID>>   LoadSoundEffects(int index) const;

private:
    const nlohmann::json&                   sceneJSON;
    nlohmann::json                          layerIndexJSON;
    nlohmann::json                          layersJSON;
    std::vector<nlohmann::json>             layerJSONList;
};