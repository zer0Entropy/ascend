#include "../include/scene/parser.hpp"
#include "../include/resource/texture.hpp"

SceneParser::SceneParser(const nlohmann::json& json):
    sceneJSON{json} {
    
}

std::vector<std::string> SceneParser::LoadLayerIndex() {
    std::vector<std::string>        layerNames;
    const auto&                     findLayerIndex{sceneJSON.find("layerIndex")};
    if(findLayerIndex != sceneJSON.end()) {
        layerIndexJSON = findLayerIndex.value();
        for(const auto& layerName : layerIndexJSON) {
            layerNames.push_back(layerName.template get<std::string>());
        }
    }
    return layerNames;
}

std::vector<Layer> SceneParser::LoadLayers() {
    const auto&                     findLayers(sceneJSON.find("layers"));
    if(findLayers != sceneJSON.end()) {
        layersJSON =                findLayers.value();
        for(const auto& layerJSON : layersJSON) {
            layerJSONList.push_back(layerJSON);
        }
    }
    const auto                  layerNames{LoadLayerIndex()};
    std::vector<Layer>          layerList;
    for(int index = 0; index < layerJSONList.size(); ++index) {
        Layer&                  layer{*layerList.insert(layerList.end(), Layer{})};
        layer.index =           index;
        layer.name =            layerNames.at(index);
        layer.typeID =          LoadLayerType(index);
        layer.resources =       LoadResources(index);
        layer.boundingBoxes =   LoadBoundingBoxes(index);
        layer.spriteAttachments = LoadSpriteAttachments(index);
        layer.textAttachments = LoadTextAttachments(index);
        layer.textContents =    LoadTextContents(index);
        layer.fontParameters =  LoadFontParam(index);
        layer.labelAlignments = LoadAlignments(index);
        layer.textureSwitches = LoadTextureSwitches(index);
        layer.soundEffects =    LoadSoundEffects(index);
        layer.backgroundMusicID = LoadBackgroundMusic(index);
        layer.musicPlaybackParam = LoadPlaybackParam(index);
    }
    return layerList;
}

Layer::TypeID SceneParser::LoadLayerType(int index) {
    const auto&                     json{layerJSONList.at(index)};
    const auto&                     findType{json.find("type")};
    if(findType != json.end()) {
        std::string                 typeName{findType.value().template get<std::string>()};
        if(typeName.compare("music") == 0) {
            return Layer::TypeID::Music;
        }
        else if(typeName.compare("decoration") == 0) {
            return Layer::TypeID::Decoration;
        }
        else if(typeName.compare("menuButton") == 0) {
            return Layer::TypeID::MenuButton;
        }
        else if(typeName.compare("menuLabel") == 0) {
            return Layer::TypeID::MenuLabel;
        }
        else if(typeName.compare("titleBarLabel") == 0) {
            return Layer::TypeID::TitleBarLabel;
        }
    }
    return Layer::TypeID::Null;
}

std::vector<ResourceToken> SceneParser::LoadResources(int index) {
    std::vector<ResourceToken>      resourceList;
    const auto&                     json{layerJSONList.at(index)};
    const auto&                     findResources{json.find("resources")};
    if(findResources != json.end()) {
        const auto&                 resourcesJSON{findResources.value()};
        const auto&                 findFonts{resourcesJSON.find("fonts")};
        const auto&                 findTextures{resourcesJSON.find("textures")};
        const auto&                 findMusics{resourcesJSON.find("musics")};
        const auto&                 findSounds{resourcesJSON.find("sounds")};
        if(findFonts != resourcesJSON.end()) {
            const auto              fontList{LoadFonts(findFonts.value())};
            resourceList.insert(resourceList.end(), fontList.begin(), fontList.end());
        }
        if(findTextures != resourcesJSON.end()) {
            const auto              textureList{LoadTextures(findTextures.value())};
            resourceList.insert(resourceList.end(), textureList.begin(), textureList.end());
        }
        if(findMusics != resourcesJSON.end()) {
            const auto              musicList{LoadMusics(findMusics.value())};
            resourceList.insert(resourceList.end(), musicList.begin(), musicList.end());
        }
        if(findSounds != resourcesJSON.end()) {
            const auto              soundList{LoadSounds(findSounds.value())};
            resourceList.insert(resourceList.end(), soundList.begin(), soundList.end());
        }
    }
    return resourceList;
}

std::vector<ResourceToken> SceneParser::LoadFonts(const nlohmann::json& json) const {
    std::vector<ResourceToken>          fontList;
    for(const auto& fontJSON : json) {
        const auto& findID{fontJSON.find("id")};
        const auto& findPath{fontJSON.find("path")};
        if(findID != fontJSON.end() && findPath != fontJSON.end()) {
            fontList.push_back(
                ResourceToken{
                    findID.value().template get<std::string>(),
                    findPath.value().template get<std::string>(),
                    Resource::TypeID::Font
                });
        }
    }
    return fontList;
}

std::vector<ResourceToken> SceneParser::LoadTextures(const nlohmann::json& json) const {
    std::vector<ResourceToken>                      textureList;
    for(const auto& textureJSON : json) {
        const auto&                                 findID{textureJSON.find("id")};
        const auto&                                 findPath{textureJSON.find("path")};
        const auto&                                 findSourceRect{textureJSON.find("sourceRect")};
        const auto&                                 findStyle{textureJSON.find("style")};
        const auto&                                 findRepeatHorizontal{textureJSON.find("repeatHorizontal")};
        const auto&                                 findRepeatVertical{textureJSON.find("repeatVertical")};
        const auto&                                 findSize{textureJSON.find("size")};
        const auto&                                 findSources{textureJSON.find("sources")};
        const auto&                                 findDestinations{textureJSON.find("destinations")};

        sf::IntRect                                 sourceRect{0, 0, 0, 0};
        if(findSourceRect != textureJSON.end()) {
            const auto& sourceRectJSON{findSourceRect.value()};
            const auto& findLeft{sourceRectJSON.find("left")};
            const auto& findTop{sourceRectJSON.find("top")};
            const auto& findWidth{sourceRectJSON.find("width")};
            const auto& findHeight{sourceRectJSON.find("height")};
            if(     findLeft != sourceRectJSON.end() && findTop != sourceRectJSON.end()
                &&  findWidth != sourceRectJSON.end() && findHeight != sourceRectJSON.end()) {
                sourceRect.left = findLeft.value();
                sourceRect.top = findTop.value();
                sourceRect.width = findWidth.value();
                sourceRect.height = findHeight.value();
            }
        }

        Texture::Style      style{Texture::Style::Simple};
        if(findStyle != textureJSON.end()) {
            style = Texture::GetStyle(findStyle.value().template get<std::string>());
        }
        
        if(findID != textureJSON.end() && findPath != textureJSON.end()) {            
            if(style == Texture::Style::Simple) {
                textureList.push_back(
                    ResourceToken{
                        findID.value().template get<std::string>(),
                        findPath.value().template get<std::string>(),
                        Resource::TypeID::SimpleTexture,
                        ResourceToken::TextureParams{sourceRect}
                    });
            }
            else if(style == Texture::Style::Repeating) {
                if(     findRepeatHorizontal != textureJSON.end()
                    &&  findRepeatVertical != textureJSON.end()) {
                    textureList.push_back(
                        ResourceToken{
                            findID.value().template get<std::string>(),
                            findPath.value().template get<std::string>(),
                            Resource::TypeID::RepeatingTexture,
                            ResourceToken::TextureParams{
                                findRepeatHorizontal.value(),
                                findRepeatVertical.value(),
                                sourceRect
                            }
                        });
                }
                else if(findRepeatHorizontal != textureJSON.end()) {
                    textureList.push_back(
                        ResourceToken{
                            findID.value().template get<std::string>(),
                            findPath.value().template get<std::string>(),
                            Resource::TypeID::RepeatingTexture,
                            ResourceToken::TextureParams{
                                findRepeatHorizontal.value(),
                                0,
                                sourceRect
                            }
                        });
                }
                else if(findRepeatVertical != textureJSON.end()) {
                    textureList.push_back(
                        ResourceToken{
                            findID.value().template get<std::string>(),
                            findPath.value().template get<std::string>(),
                            Resource::TypeID::RepeatingTexture,
                            ResourceToken::TextureParams{
                                0,
                                findRepeatVertical.value(),
                                sourceRect
                            }
                        });
                }
            }
        }
        else if(style == Texture::Style::Composite) {
            if(     findID != textureJSON.end()
                &&  findSize != textureJSON.end()
                &&  findSources != textureJSON.end()
                &&  findDestinations != textureJSON.end()) {
                sf::Vector2u                        size{0, 0};
                const auto&                         sizeJSON{findSize.value()};
                const auto&                         findWidth{sizeJSON.find("width")};
                const auto&                         findHeight{sizeJSON.find("height")};
                if(findWidth != sizeJSON.end() && findHeight != sizeJSON.end()) {
                    size.x =                        findWidth.value();
                    size.y =                        findHeight.value();
                }
                std::vector<ResourceID>             sourceList;
                std::vector<sf::Vector2u>           destList;
                for(const auto& source : findSources.value()) {
                    sourceList.push_back((ResourceID)source.template get<std::string>());
                }
                for(const auto& dest : findDestinations.value()) {
                    const auto&                     findX{dest.find("x")};
                    const auto&                     findY{dest.find("y")};
                    if(findX != dest.end() && findY != dest.end()) {
                        destList.push_back(sf::Vector2u{findX.value(), findY.value()});
                    }
                }
                if(sourceList.size() > 0 && destList.size() > 0 && sourceList.size() == destList.size()) {
                    textureList.push_back(
                        ResourceToken{
                            findID.value().template get<std::string>(),
                            "",
                            Resource::TypeID::CompositeTexture,
                            ResourceToken::TextureParams{
                                size,
                                sourceList,
                                destList
                            }
                        });
                }
            }
        }
    }
    return textureList;
}

std::vector<ResourceToken> SceneParser::LoadMusics(const nlohmann::json& json) const {
    std::vector<ResourceToken>          musicList;
    for(const auto& musicJSON : json) {
        const auto& findID{musicJSON.find("id")};
        const auto& findPath{musicJSON.find("path")};
        if(findID != musicJSON.end() && findPath != musicJSON.end()) {
            musicList.push_back(
                ResourceToken{
                    findID.value().template get<std::string>(),
                    findPath.value().template get<std::string>(),
                    Resource::TypeID::Music
                });
        }
    }
    return musicList;
}

std::vector<ResourceToken> SceneParser::LoadSounds(const nlohmann::json& json) const {
    std::vector<ResourceToken>          soundList;
    for(const auto& soundJSON : json) {
        const auto& findID{soundJSON.find("id")};
        const auto& findPath{soundJSON.find("path")};
        if(findID != soundJSON.end() && findPath != soundJSON.end()) {
            soundList.push_back(
                ResourceToken{
                    findID.value().template get<std::string>(),
                    findPath.value().template get<std::string>(),
                    Resource::TypeID::Sound
                });
        }
    }
    return soundList;
}

std::vector<sf::Rect<unsigned int>> SceneParser::LoadBoundingBoxes(int index) const {
    const auto&                                 json{layerJSONList.at(index)};
    std::vector<sf::Rect<unsigned int>>         boundsList;
    const auto&                                 findBounds{json.find("bounds")};
    if(findBounds == json.end()) {
        return boundsList;
    }
    const auto&                                 boundsJSON{findBounds.value()};
    for(const auto& boundingBoxJSON : boundsJSON) {
        const auto&                             findBoundsRect{boundingBoxJSON.find("boundsRect")};
        const auto&                             rectangle{findBoundsRect.value()};
        const auto&                             findLeft{rectangle.find("left")};
        const auto&                             findTop{rectangle.find("top")};
        const auto&                             findWidth{rectangle.find("width")};
        const auto&                             findHeight{rectangle.find("height")};
        if(     findLeft != rectangle.end() &&  findTop != rectangle.end()
            &&  findWidth != rectangle.end() &&  findHeight != rectangle.end()) {
            boundsList.push_back(sf::Rect<unsigned int>{
                findLeft.value(),
                findTop.value(),
                findWidth.value(),
                findHeight.value()
            });
        }
    }
    return boundsList;
}

std::vector<ResourceID> SceneParser::LoadSpriteAttachments(int index) const {
    const auto&                                 json{layerJSONList.at(index)};
    std::vector<ResourceID>                     attachmentList;
    const auto&                                 findSprites{json.find("sprites")};
    if(findSprites == json.end()) {
        return attachmentList;
    }
    const auto&                                 spritesJSON{findSprites.value()};
    for(const auto& spriteJSON : spritesJSON) {
        const auto&                             findAttachments{spriteJSON.find("attachments")};
        if(findAttachments != spriteJSON.end()) {
            const auto&                         attachmentsJSON{findAttachments.value()};
            const auto&                         findTextures{attachmentsJSON.find("textures")};
            if(findTextures != attachmentsJSON.end()) {
                const auto&                     texturesJSON{findTextures.value()};
                const auto&                     textureIter{texturesJSON.begin()};
                attachmentList.push_back((*textureIter).template get<std::string>());
            }
        }
    }
    return attachmentList;
}

std::vector<ResourceID> SceneParser::LoadTextAttachments(int index) const {
    const auto&                                     json{layerJSONList.at(index)};
    std::vector<ResourceID>                         attachmentList;
    const auto&                                     findTexts{json.find("texts")};
    if(findTexts == json.end()) {
        return attachmentList;
    }
    const auto&                                     textsJSON{findTexts.value()};
    for(const auto& textJSON : textsJSON) {
        const auto&                                 findAttachments{textJSON.find("attachments")};
        if(findAttachments != textJSON.end()) {
            const auto&                             attachmentsJSON{findAttachments.value()};
            const auto&                             findFonts{attachmentsJSON.find("fonts")};
            if(findFonts != attachmentsJSON.end()) {
                const auto&                         fontsJSON{findFonts.value()};
                const auto&                         fontIter{fontsJSON.begin()};
                attachmentList.push_back((*fontIter).template get<std::string>());
            }
        }
    }
    return attachmentList;
}

std::vector<std::string> SceneParser::LoadTextContents(int index) const {
    const auto&                                     json{layerJSONList.at(index)};
    std::vector<ResourceID>                         contentsList;
    const auto&                                     findTexts{json.find("texts")};
    if(findTexts == json.end()) {
        return contentsList;
    }
    const auto&                                     textsJSON{findTexts.value()};
    for(const auto& textJSON : textsJSON) {
        const auto&                                 findContents{textJSON.find("contents")};
        if(findContents != textJSON.end()) {
            contentsList.push_back(findContents.value().template get<std::string>());
        }
    }
    return contentsList;
}

std::vector<Text::FontParameters> SceneParser::LoadFontParam(int index) const {
    const auto&                                     json{layerJSONList.at(index)};
    std::vector<Text::FontParameters>               paramList;
    const auto&                                     findTexts{json.find("texts")};
    if(findTexts == json.end()) {
        return paramList;
    }
    const auto&                                     textsJSON{findTexts.value()};
    for(const auto& textJSON : textsJSON) {
        const auto&                                 findParam{textJSON.find("fontParameters")};
        if(findParam != textJSON.end()) {
            const auto&                             paramJSON{findParam.value()};
            const auto&                             findID{paramJSON.find("id")};
            const auto&                             findFontSize{paramJSON.find("fontSize")};
            const auto&                             findOutlineColor{paramJSON.find("outlineColor")};
            const auto&                             findFillColor{paramJSON.find("fillColor")};
            if(     findID != paramJSON.end()
                &&  findFontSize != paramJSON.end()
                &&  findOutlineColor != paramJSON.end()
                &&  findFillColor != paramJSON.end()) {
                paramList.push_back(Text::FontParameters{
                    findID.value().template get<std::string>(),
                    findFontSize.value(),
                    GetColorFromHex(findOutlineColor.value().template get<std::string>()),
                    GetColorFromHex(findFillColor.value().template get<std::string>())
                });
            }
        }
    }
    return paramList;
}

ResourceID SceneParser::LoadBackgroundMusic(int index) const {
    const auto&                                     json{layerJSONList.at(index)};
    const auto&                                     findBackgroundMusic{json.find("backgroundMusic")};
    if(findBackgroundMusic == json.end()) {
        return ResourceID{""};
    }
    const auto&                                     bgMusicJSON{findBackgroundMusic.value()};
    const auto&                                     findID{bgMusicJSON.find("id")};
    if(findID != bgMusicJSON.end()) {
        return (ResourceID)findID.value().template get<std::string>();
    }
    return ResourceID{""};
}

Music::PlaybackParam SceneParser::LoadPlaybackParam(int index) const {
    Music::PlaybackParam                            param{false, 100.0};
    const auto&                                     json{layerJSONList.at(index)};
    const auto&                                     findBackgroundMusic{json.find("backgroundMusic")};
    if(findBackgroundMusic == json.end()) {
        return param;
    }
    const auto&                                     bgMusicJSON{findBackgroundMusic.value()};
    const auto&                                     findLoop{bgMusicJSON.find("loop")};
    const auto&                                     findVolume{bgMusicJSON.find("volume")};
    if(findLoop != bgMusicJSON.end()) {
        param.loop = findLoop.value();
    }
    if(findVolume != bgMusicJSON.end()) {
        param.volume = findVolume.value();
    }
    return param;
}

std::vector<Alignment> SceneParser::LoadAlignments(int index) const {
    const auto&                                     json{layerJSONList.at(index)};
    std::vector<Alignment>                          alignmentList;
    const auto&                                     findAlignables{json.find("alignments")};
    if(findAlignables == json.end()) {
        return alignmentList;
    }
    const auto&                                     alignmentsJSON{findAlignables.value()};
    for(const auto& alignLabel : alignmentsJSON) {
        const auto&                                 findAlign{alignLabel.find("align")};
        if(findAlign != alignLabel.end()) {
            alignmentList.push_back(GetAlignment(findAlign.value().template get<std::string>()));
        }
    }
    return alignmentList;
}

std::vector<std::pair<Event::TypeID, ResourceID>> SceneParser::LoadTextureSwitches(int index) const {
    const auto&                                     json{layerJSONList.at(index)};
    std::vector<std::pair<Event::TypeID, ResourceID>>   textureSwitchList;
    const auto&                                     findTextureSwitches(json.find("textureSwitches"));
    if(findTextureSwitches == json.end()) {
        return textureSwitchList;
    }
    const auto&                                     textureSwitchesJSON{findTextureSwitches.value()};
    for(const auto& textureSwitch : textureSwitchesJSON) {
        const auto&                                 findTriggerEvent{textureSwitch.find("triggerEvent")};
        const auto&                                 findTextureID{textureSwitch.find("textureID")};
        if(findTriggerEvent != textureSwitch.end() && findTextureID != textureSwitch.end()) {
            textureSwitchList.push_back(
                std::make_pair(
                    Event::GetTypeID(findTriggerEvent.value().template get<std::string>()),
                    findTextureID.value().template get<std::string>()));
        }
    }
    return textureSwitchList;
}

std::vector<std::pair<Event::TypeID, ResourceID>> SceneParser::LoadSoundEffects(int index) const {
    const auto&                                     json{layerJSONList.at(index)};
    std::vector<std::pair<Event::TypeID, ResourceID>>   soundEffectList;
    const auto&                                     findSoundEffects(json.find("soundEffects"));
    if(findSoundEffects == json.end()) {
        return soundEffectList;
    }
    const auto&                                     textureSwitchesJSON{findSoundEffects.value()};
    for(const auto& textureSwitch : textureSwitchesJSON) {
        const auto&                                 findTriggerEvent{textureSwitch.find("triggerEvent")};
        const auto&                                 findTextureID{textureSwitch.find("soundID")};
        if(findTriggerEvent != textureSwitch.end() && findTextureID != textureSwitch.end()) {
            soundEffectList.push_back(
                std::make_pair(
                    Event::GetTypeID(findTriggerEvent.value().template get<std::string>()),
                    findTextureID.value().template get<std::string>()));
        }
    }
    return soundEffectList;
}