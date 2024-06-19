#include "../include/scene/scene.hpp"
#include "../include/core/app.hpp"

Scene::Scene(EntityMgr& entMgr, ResourceMgr& resMgr):
    ISerializeable{},
    ILogMsgPublisher{},
    entityMgr{entMgr},
    resourceMgr{resMgr},
    optionSelector{this} {

    auto& eventSystem{*Application::GetInstance().GetEventSystem()};
    for(int index = 0; index < Event::NumEventTypes; ++index) {
        eventSystem.Subscribe(&optionSelector, (Event::TypeID)index);
    }
}

void Scene::LoadFromJSON(const nlohmann::json& json) {
    const auto&                 findLayerIndex{json.find("layerIndex")};
    const auto&                 layerList{findLayerIndex.value()};
    LoadLayerIndex(layerList);
    
    const auto&                 findLayers{json.find("layers")};
    const auto&                 layersJSON{findLayers.value()};
    for(const auto& layerJSON : layersJSON) {
        LoadLayer(layerJSON);
    }

    for(int index = 0; index < layers.size(); ++index) {
        Layer& layer{layers[index]};
        switch(layer.typeID) {
            case Layer::TypeID::Music: {
                // Nothing to do ; already loaded above
            } break;
            case Layer::TypeID::Decoration: {
                CreateDecorations(layer);
            } break;
            case Layer::TypeID::MenuButton: {
                CreateMenuButtons(layer);
            } break;
            case Layer::TypeID::MenuLabel: {
                CreateMenuLabels(layer);
            } break;
        }
    }
}

nlohmann::json Scene::SaveToJSON() const {
    return nlohmann::json{};
}

SpriteMgr& Scene::GetSpriteMgr() const {
    return const_cast<SpriteMgr&>(spriteMgr);
}

TextMgr& Scene::GetTextMgr() const {
    return const_cast<TextMgr&>(textMgr);
}

BoundingBoxMgr& Scene::GetBoundingBoxMgr() const {
    return const_cast<BoundingBoxMgr&>(boundingBoxMgr);
}

HoverableMgr& Scene::GetHoverableMgr() const {
    return const_cast<HoverableMgr&>(hoverableMgr);
}

TextureSwitcherMgr& Scene::GetTextureSwitcherMgr() const {
    return const_cast<TextureSwitcherMgr&>(textureSwitcherMgr);
}

LeftClickableMgr& Scene::GetLeftClickableMgr() const {
    return const_cast<LeftClickableMgr&>(leftClickableMgr);
}

const Menu& Scene::GetMenu() const {
    return menu;
}

void Scene::SetSelectedMenuOption(int index) {
    if(index < 0) {
        menu.selectedOption = nullptr;
    }
    else if(index < menu.options.size()) {
        menu.selectedOption = &menu.options.at(index);
    }
}

void Scene::ConfirmSelectedMenuOption() {
    if(!menu.selectedOption) {
        return;
    }
    auto& eventSystem{*Application::GetInstance().GetEventSystem()};
    MenuOption& selection{*menu.selectedOption};
    if(selection.name.compare("New Game") == 0) {
        eventSystem.Enqueue(Event{Event::TypeID::NewGameStarted, selection.widget});
    }
    else if(selection.name.compare("Load Game") == 0) {
        eventSystem.Enqueue(Event{Event::TypeID::LoadGameStarted, selection.widget});
    }
    else if(selection.name.compare("Options") == 0) {
        eventSystem.Enqueue(Event{Event::TypeID::OptionsStarted, selection.widget});
    }
    else if(selection.name.compare("Quit Game") == 0) {
        eventSystem.Enqueue(Event{Event::TypeID::QuitGameStarted, selection.widget});
    }
}

Scene::Layer* Scene::GetLayer(std::string_view name) {
    for(auto iter = layers.begin(); iter != layers.end(); ++iter) {
        if(iter->name.compare(name) == 0) {
            return &(*iter);
        }
    }
    return nullptr;
}

void Scene::LoadLayerIndex(const nlohmann::json& json) {
    unsigned int index = 0;
    for(const auto& layerName : json) {
        layers.push_back(Layer{});
        Layer&                  layer{layers.back()};
        layer.index = index;
        layerName.get_to(layer.name);
    }
}

void Scene::LoadLayer(const nlohmann::json& json) {
    ResourceMgr&                resourceMgr{Application::GetInstance().GetResourceMgr()};
    const auto&                 findName{json.find("name")};
    if(findName != json.end()) {
        std::string             name{findName.value().template get<std::string>()};
        Layer*                  layerPtr{GetLayer(name)};
        if(!layerPtr)           {return;}
        Layer&                  layer{*layerPtr};
        const auto&             findType{json.find("type")};
        if(findType != json.end()) {
            std::string         typeName{findType.value().template get<std::string>()};
            if(typeName.compare("decoration") == 0) {
                layer.typeID = Layer::TypeID::Decoration;
            }
            else if(typeName.compare("menuButton") == 0) {
                layer.typeID = Layer::TypeID::MenuButton;
            }
            else if(typeName.compare("menuLabel") == 0) {
                layer.typeID = Layer::TypeID::MenuLabel;
            }

            const auto&             findResources{json.find("resources")};
            const auto&             findMusic{json.find("music")};
            const auto&             findBounds{json.find("bounds")};
            const auto&             findSprites{json.find("sprites")};
            const auto&             findTexts{json.find("texts")};
            const auto&             findTextureSwitches{json.find("textureSwitches")};
            const auto&             findAlignLabels{json.find("alignLabels")};

            if(findResources != json.end()) {
                const auto&         resourcesJSON{findResources.value()};
                LoadResourcesJSON(resourcesJSON, layer);
                LoadResources(layer);
            }

            if(findBounds != json.end()) {
                const auto&         boundsJSON{findBounds.value()};
                LoadBoundingBoxes(boundsJSON, layer);
            }
            if(findSprites != json.end()) {
                const auto&         spritesJSON{findSprites.value()};
                LoadSprites(spritesJSON, layer);
            }
            if(findTexts != json.end()) {
                const auto&         textsJSON{findTexts.value()};
                LoadTexts(textsJSON, layer);
            }
            if(findTextureSwitches != json.end()) {
                const auto&         textureSwitches{findTextureSwitches.value()};
                LoadTextureSwitches(textureSwitches, layer);
            }
            if(findAlignLabels != json.end()) {
                const auto&         alignLabelsJSON{findAlignLabels.value()};
                LoadLabelAlignments(alignLabelsJSON, layer);
            }
            if(findMusic != json.end()) {
                const auto&         musicJSON{findMusic.value()};
                LoadMusic(musicJSON, layer);
            }
        }
    }   
}

void Scene::LoadResourcesJSON(const nlohmann::json& json, Layer& layer) {
    const auto&                 findFonts{json.find("fonts")};
    if(findFonts != json.end()) {
        LoadFontsJSON(findFonts.value(), layer);
    }

    const auto&                 findTextures{json.find("textures")};
    if(findTextures != json.end()) {
        LoadTexturesJSON(findTextures.value(), layer);
    }

    const auto&                 findMusics{json.find("musics")};
    if(findMusics != json.end()) {
        LoadMusicsJSON(findMusics.value(), layer);
    }

    const auto&                 findSounds{json.find("sounds")};
    if(findSounds != json.end()) {
        LoadSoundsJSON(findSounds.value(), layer);
    }
}

void Scene::LoadFontsJSON(const nlohmann::json& json, Layer& layer) {
    for(const auto& fontJSON : json) {
        const auto&         findID{fontJSON.find("id")};
        const auto&         findPath{fontJSON.find("path")};
        if(     findID != fontJSON.end()
            &&  findPath != fontJSON.end()) {
            layer.fonts.push_back(ResourceToken{
                                    findID.value().template get<std::string>(),
                                    findPath.value().template get<std::string>()});
        }
    }
}

void Scene::LoadTexturesJSON(const nlohmann::json& json, Layer& layer) {
    for(const auto& textureJSON : json) {
        // Determine TextureStyle
        Texture::Style              style{Texture::Style::Simple};
        const auto&                 findStyle{textureJSON.find("style")};
        if(findStyle != textureJSON.end()) {
            std::string             name{findStyle.value().template get<std::string>()};
            style =                 Texture::GetStyle(name);
        }
        switch(style) {
            case Texture::Style::Simple: {
                LoadSimpleTextureJSON(textureJSON, layer);
            } break;
            case Texture::Style::Composite: {
                LoadCompositeTextureJSON(textureJSON, layer);
            } break;
            case Texture::Style::Repeating: {
                LoadRepeatingTextureJSON(textureJSON, layer);
            } break;
        }
    }
}

void Scene::LoadMusicsJSON(const nlohmann::json& json, Layer& layer) {
    for(const auto& musicJSON : json) {
        const auto&             findID{musicJSON.find("id")};
        const auto&             findPath{musicJSON.find("path")};
        if(     findID != musicJSON.end()
            &&  findPath != musicJSON.end()) {
            layer.musics.push_back(ResourceToken{
                findID.value().template get<std::string>(),
                findPath.value().template get<std::string>()});
        }
    }
}

void Scene::LoadSoundsJSON(const nlohmann::json& json, Layer& layer) {
    for(const auto& soundJSON : json) {
        const auto&             findID{soundJSON.find("id")};
        const auto&             findPath{soundJSON.find("path")};
        if(     findID != soundJSON.end()
            &&  findPath != soundJSON.end()) {
            layer.sounds.push_back(ResourceToken{
                findID.value().template get<std::string>(),
                findPath.value().template get<std::string>()});
        }
    }
}

sf::IntRect Scene::LoadRectFromJSON(const nlohmann::json& json) const {
    sf::Rect            rectangle{0, 0, 0, 0};
    const auto& findLeft{json.find("left")};
    const auto& findTop{json.find("top")};
    const auto& findWidth{json.find("width")};
    const auto& findHeight{json.find("height")};
    if(     findLeft != json.end()
        &&  findTop != json.end()
        &&  findWidth != json.end()
        &&  findHeight != json.end()) {
        rectangle.left =    findLeft.value();
        rectangle.top =     findTop.value();
        rectangle.width =   findWidth.value();
        rectangle.height =  findHeight.value();
    }
    return rectangle;
}

void Scene::LoadSimpleTextureJSON(const nlohmann::json& json, Layer& layer) {
    // Determine sourceRect
    sf::IntRect             sourceRect{0, 0, 0, 0};
    const auto&             findSourceRect{json.find("sourceRect")};
    if(findSourceRect != json.end()) {
        sourceRect =        LoadRectFromJSON(findSourceRect.value());
    }
    // SimpleTexture requires an ID and a Path, with an optional sourceRect
    const auto&             findID{json.find("id")};
    const auto&             findPath{json.find("path")};
    if(     findID != json.end()
        &&  findPath != json.end()) {
        layer.textures.push_back(ResourceToken{
                                                findID.value().template get<std::string>(),
                                                findPath.value().template get<std::string>(),
                                                sourceRect}); // sourceRect defaults to {0, 0, 0, 0}
    }
}

void Scene::LoadCompositeTextureJSON(const nlohmann::json& json, Layer& layer) {
    /*  CompositeTexture requires:
            ResourceID
            size
            sourceList
            destList
    */
    const auto&                     findID{json.find("id")};
    const auto&                     findSize{json.find("size")};
    const auto&                     findSources{json.find("sources")};
    const auto&                     findDestinations{json.find("destinations")};
    if(     findID != json.end()
        &&  findSize != json.end()
        &&  findSources != json.end()
        &&  findDestinations != json.end()) {
        const auto&                 sizeJSON{findSize.value()};
        const auto&                 findWidth{sizeJSON.find("width")};
        const auto&                 findHeight{sizeJSON.find("height")};
        sf::Vector2u                size{
            findWidth.value(),
            findHeight.value()  
        };
        std::vector<ResourceID>     sourceList;
        for(const auto& source : findSources.value()) {
            ResourceID sourceID{(ResourceID)source.template get<std::string>()};
            sourceList.push_back(sourceID);
        }
        std::vector<sf::Vector2u>   destList;
        const auto&                 destJSON{findDestinations.value()};
        for(const auto& dest: destJSON) {
            const auto&             destX{dest.find("x")};
            const auto&             destY{dest.find("y")};
            if(     destX != destJSON.end()
                &&  destY != destJSON.end()) {
                destList.push_back(sf::Vector2u{destX.value(), destY.value()});
            }
        }
        layer.compositeTextures.push_back(CompositeTextureToken{
                                                            findID.value().template get<std::string>(),
                                                            size,
                                                            sourceList,
                                                            destList});
    }
}

void Scene::LoadRepeatingTextureJSON(const nlohmann::json& json, Layer& layer) {
    // Determine sourceRect
    sf::IntRect             sourceRect{0, 0, 0, 0};
    const auto&             findSourceRect{json.find("sourceRect")};
    if(findSourceRect != json.end()) {
        sourceRect = LoadRectFromJSON(findSourceRect.value());
    }

    /* RepeatingTexture requires either:
        (a)     ResourceID
                path
                sourceRect (optional)
                Orientation
                numRepetitions
    OR
        (b)     ResourceID
                path
                sourceRect (optional)
                repeatRect
    */ 
    const auto&             findID{json.find("id")};
    const auto&             findPath{json.find("path")};
    const auto&             findOrientation{json.find("orientation")};
    const auto&             findRepetitions{json.find("repeat")};
    const auto&             findRepeatRect{json.find("repeatRect")};
    if(     findID != json.end()
        &&  findPath != json.end()
        &&  findOrientation != json.end()
        &&  findRepetitions != json.end()) {
        Orientation orientation{GetOrientation(findOrientation.value().template get<std::string>())};
        layer.repeatingTextures.push_back(RepeatingTextureToken{
                                            findID.value().template get<std::string>(),
                                            findPath.value().template get<std::string>(),
                                            sourceRect,
                                            orientation,
                                            findRepetitions.value()});
    }
    else if(findRepeatRect != json.end()) {
        const auto&         repeatRect{findRepeatRect.value()};
        const auto&         findX{repeatRect.find("x")};
        const auto&         findY{repeatRect.find("y")};
        if(     findID != json.end()
            &&  findPath != json.end()
            &&  findX != repeatRect.end()
            &&  findY != repeatRect.end()) {
            layer.repeatingTextures.push_back(RepeatingTextureToken{
                                            findID.value().template get<std::string>(),
                                            findPath.value().template get<std::string>(),
                                            sourceRect,
                                            sf::Vector2u{findX.value(), findY.value()}});
        }
    }
}


void Scene::LoadResources(Layer& layer) {
    for(const auto& font : layer.fonts) {
        resourceMgr.LoadFont(font.id, font.path);
    }
    for(const auto& texture : layer.textures) {
        resourceMgr.LoadTexture(texture.id, texture.path, texture.sourceRect);
    }
    for(const auto& repeatingTexture : layer.repeatingTextures) {
        if(repeatingTexture.isRect) {
            resourceMgr.LoadTexture(    repeatingTexture.id,
                                        repeatingTexture.path,
                                        repeatingTexture.repeatRect,
                                        repeatingTexture.sourceRect);
        }
        else {
            resourceMgr.LoadTexture(    repeatingTexture.id,
                                        repeatingTexture.path,
                                        repeatingTexture.orientation,
                                        repeatingTexture.numRepetitions,
                                        repeatingTexture.sourceRect);
        }
    }
    for(const auto& compositeTexture : layer.compositeTextures) {
        resourceMgr.LoadTexture(    compositeTexture.id,
                                    compositeTexture.size,
                                    compositeTexture.sourceTextureIDs,
                                    compositeTexture.destinations);
    }
    for(const auto& music : layer.musics) {
        resourceMgr.LoadMusic(music.id, music.path);
    }
    for(const auto& sound : layer.sounds) {
        resourceMgr.LoadSound(sound.id, sound.path);
    }
}

void Scene::LoadBoundingBoxes(const nlohmann::json& json, Layer& layer) {
    for(const auto& boundsJSON : json) {
        const auto&         findBoundsRect{boundsJSON.find("boundsRect")};
        const auto&         rectangle{findBoundsRect.value()};
        const auto&         findLeft{rectangle.find("left")};
        const auto&         findTop{rectangle.find("top")};
        const auto&         findWidth{rectangle.find("width")};
        const auto&         findHeight{rectangle.find("height")};
        if(     findLeft != rectangle.end()
            &&  findTop != rectangle.end()
            &&  findWidth != rectangle.end()
            &&  findHeight != rectangle.end()) {
            layer.boundingBoxes.push_back(sf::Rect<unsigned int>{
                findLeft.value(),
                findTop.value(),
                findWidth.value(),
                findHeight.value()
            });
        }
    }
}

void Scene::LoadSprites(const nlohmann::json& json, Layer& layer) {
    ResourceMgr&                    resourceMgr{Application::GetInstance().GetResourceMgr()};
    for(const auto& spriteJSON : json) {
        const auto&                 findAttachments{spriteJSON.find("attachments")};
        const auto&                 attachmentsJSON{findAttachments.value()};
        const auto&                 findTextures{attachmentsJSON.find("textures")};
        if(findTextures != attachmentsJSON.end()) {
            const auto&             texturesJSON{findTextures.value()};
            ResourceID              textureID{texturesJSON.begin()->template get<std::string>()};
            Texture*                texture{resourceMgr.GetTexture(textureID)};
            layer.spriteAttachments.push_back(texture);
        }
    }
}

void Scene::LoadTexts(const nlohmann::json& json, Layer& layer) {
    ResourceMgr&                    resourceMgr{Application::GetInstance().GetResourceMgr()};
    for(const auto& textJSON : json) {
        const auto&                 findContents{textJSON.find("contents")};
        if(findContents != textJSON.end()) {
            layer.textContents.push_back(findContents.value().template get<std::string>());
        }
        const auto&                 findFontParam{textJSON.find("fontParameters")};
        if(findFontParam != textJSON.end()) {
            const auto&             fontParamJSON{findFontParam.value()};
            const auto&             findFontID{fontParamJSON.find("id")};
            const auto&             findFontSize{fontParamJSON.find("fontSize")};
            const auto&             findOutlineColor{fontParamJSON.find("outlineColor")};
            const auto&             findFillColor{fontParamJSON.find("fillColor")};
            if(     findFontID != fontParamJSON.end()
                &&  findFontSize != fontParamJSON.end()
                &&  findOutlineColor != fontParamJSON.end()
                &&  findFillColor != fontParamJSON.end()) {
                layer.fontParameters.push_back(
                    Text::FontParameters{
                        findFontID.value().template get<std::string>(),
                        findFontSize.value(),
                        GetColorFromHex(findOutlineColor.value().template get<std::string>()),
                        GetColorFromHex(findFillColor.value().template get<std::string>())
                    }
                );
            }
        }
        const auto&                 findAttachments{textJSON.find("attachments")};
        const auto&                 attachmentsJSON{findAttachments.value()};
        const auto&                 findFonts{attachmentsJSON.find("fonts")};
        if(findFonts != attachmentsJSON.end()) {
            const auto&             fontsJSON{findFonts.value()};
            ResourceID              fontID{fontsJSON.begin()->template get<std::string>()};
            Font*                   font{resourceMgr.GetFont(fontID)};
            layer.textAttachments.push_back(font);
        }
    }
}

void Scene::LoadTextureSwitches(const nlohmann::json& json, Layer& layer) {
    for(const auto& textureSwitch : json) {
        const auto&                 findTriggerEvent{textureSwitch.find("triggerEvent")};
        const auto&                 findTextureID{textureSwitch.find("textureID")};
        if(     findTriggerEvent != textureSwitch.end()
            &&  findTextureID != textureSwitch.end()) {
            std::string             eventName{findTriggerEvent.value().template get<std::string>()};
            Event::TypeID           triggerEvent;
            if(eventName.compare(Event::TypeNames.at((int)Event::TypeID::CursorHoveringStarted)) == 0) {
                triggerEvent = Event::TypeID::CursorHoveringStarted;
            }
            else if(eventName.compare(Event::TypeNames.at((int)Event::TypeID::CursorHoveringStopped)) == 0) {
                triggerEvent = Event::TypeID::CursorHoveringStopped;
            }
            else if(eventName.compare(Event::TypeNames.at((int)Event::TypeID::ButtonPressStarted)) == 0) {
                triggerEvent = Event::TypeID::ButtonPressStarted;
            }
            else if(eventName.compare(Event::TypeNames.at((int)Event::TypeID::ButtonPressCompleted)) == 0) {
                triggerEvent = Event::TypeID::ButtonPressCompleted;
            }
            else if(eventName.compare(Event::TypeNames.at((int)Event::TypeID::ButtonPressAborted)) == 0) {
                triggerEvent = Event::TypeID::ButtonPressAborted;
            }
            else {
                continue;
            }
            layer.textureSwitchTriggers.push_back(std::make_pair(
                triggerEvent,
                findTextureID.value().template get<std::string>()   
            ));
        }
    }
}

void Scene::LoadLabelAlignments(const nlohmann::json& json, Layer& layer) {
    for(const auto& alignLabel : json) {
        const auto&                 findAlignment{alignLabel.find("alignment")};
        if(findAlignment != alignLabel.end()) {
            std::string             alignmentName{findAlignment.value().template get<std::string>()};
            layer.labelAlignments.push_back(GetAlignment(alignmentName));
        }
    }
}

void Scene::LoadMusic(const nlohmann::json& json, Layer& layer) {
    if(layer.musics.empty()) {
        return;
    }
    const auto&                 findLoop{json.find("loop")};
    bool                        loop{true};
    if(findLoop != json.end()) {
        loop = findLoop.value();
    }
    const ResourceToken&        musicToken{*layer.musics.begin()};
    Music* music{resourceMgr.GetMusic(musicToken.id)};
    if(music) {
        MusicSystem& musicSystem{*Application::GetInstance().GetMusicSystem()};
        musicSystem.SetMusic(musicToken.id);
        musicSystem.SetLoop(loop);
        //musicSystem.Play();
    }
}

void Scene::CreateDecorations(Layer& layer) {
    /*  A Decoration layer has:
            A list of ResourceTokens {ResourceID, path} for loading Textures
            A list of BoundingBoxes {sf::Rect<unsigned int>}
            A list of Sprites {ResourceID for Texture to attach}
    */

    ScaleRenderableMgr& scaleRenderableMgr{Application::GetInstance().GetRenderSystem()->GetScaleRenderableMgr()};
    for(int index = 0; index < layer.boundingBoxes.size(); ++index) {
        layer.entities.push_back(entityMgr.CreateEntity());
    }
    Entity firstEntity{*layer.entities.begin()};
    int index = firstEntity;
    for(const auto& boundsRect : layer.boundingBoxes) {
        boundingBoxMgr.Add((Entity)index++, boundsRect);
    }
    index = firstEntity;
    for(const auto& texture : layer.spriteAttachments) {
        spriteMgr.Add((Entity)index);
        auto& sprite{*spriteMgr.Get((Entity)index)};
        Resource* attachment{layer.spriteAttachments.at(index - firstEntity)};
        sprite.Attach(attachment);
        if(     attachment->GetTypeID() == Resource::TypeID::SimpleTexture
            ||  attachment->GetTypeID() == Resource::TypeID::CompositeTexture
            ||  attachment->GetTypeID() == Resource::TypeID::RepeatingTexture) {
            Texture* texture{dynamic_cast<Texture*>(attachment)};
            const auto& textureSize{texture->GetTexture().getSize()};
            BoundingBox& bounds{*boundingBoxMgr.Get((Entity)index)};
            if(     textureSize.x < bounds.GetWidth()
                ||  textureSize.y < bounds.GetHeight()) {
                scaleRenderableMgr.Add((Entity)index, bounds, sprite);
            }
        }
        index++;
    }
    auto& renderableMgr{Application::GetInstance().GetRenderSystem()->GetRenderableMgr()};
    for(int entityIndex = firstEntity; entityIndex < firstEntity + layer.entities.size(); ++entityIndex) {
        renderableMgr.Add(  layer.index,
                            (Entity)entityIndex,
                            *boundingBoxMgr.Get(entityIndex),
                            *spriteMgr.Get(entityIndex));
    }
}

void Scene::CreateMenuButtons(Layer& layer) {
    /*  A MenuButton layer has:
            A list of ResourceTokens {ResourceID, path} for loading Textures
            A list of BoundingBoxes {sf::Rect<unsigned int>}
            A list of Sprites {ResourceID for Texture to attach}
            A list of TextureSwitches {TriggerEventID, ResourceID} associating Events with Texture IDs
    */
    EventSystem& eventSystem{*Application::GetInstance().GetEventSystem()};
    InputSystem& inputSystem{*Application::GetInstance().GetInputSystem()};
    ScaleRenderableMgr& scaleRenderableMgr{Application::GetInstance().GetRenderSystem()->GetScaleRenderableMgr()};
    for(int index = 0; index < layer.boundingBoxes.size(); ++index) {
        layer.entities.push_back(entityMgr.CreateEntity());
    }
    Entity firstEntity{*layer.entities.begin()};
    int index = firstEntity;
    for(const auto& boundsRect : layer.boundingBoxes) {
        boundingBoxMgr.Add((Entity)index++, boundsRect);
    }
    index = firstEntity;
    for(const auto& texture : layer.spriteAttachments) {
        spriteMgr.Add((Entity)index);
        auto& sprite{*spriteMgr.Get((Entity)index)};
        Resource* attachment{layer.spriteAttachments.at(index - firstEntity)};
        sprite.Attach(attachment);
        if(     attachment->GetTypeID() == Resource::TypeID::SimpleTexture
            ||  attachment->GetTypeID() == Resource::TypeID::CompositeTexture
            ||  attachment->GetTypeID() == Resource::TypeID::RepeatingTexture) {
            Texture* texture{dynamic_cast<Texture*>(attachment)};
            const auto& textureSize{texture->GetTexture().getSize()};
            BoundingBox& bounds{*boundingBoxMgr.Get((Entity)index)};
            if(     textureSize.x < bounds.GetWidth()
                ||  textureSize.y < bounds.GetHeight()) {
                scaleRenderableMgr.Add((Entity)index, bounds, sprite);
            }
        }
        index++;
    }
    for(const auto entity : layer.entities) {
        BoundingBox* boundingBoxPtr{boundingBoxMgr.Get(entity)};
        hoverableMgr.Add(entity, *boundingBoxPtr);
        inputSystem.Subscribe(hoverableMgr.Get(entity));
        textureSwitcherMgr.Add(entity, *spriteMgr.Get(entity));
        leftClickableMgr.Add(entity, *boundingBoxPtr);
        inputSystem.Subscribe(leftClickableMgr.Get(entity));
        for(const auto& textureSwitch : layer.textureSwitchTriggers) {
            ResourceID textureID{textureSwitch.second};
            Texture* texture{resourceMgr.GetTexture(textureID)};
            auto& textureSwitcher{*textureSwitcherMgr.Get(entity)};
            textureSwitcher.Attach(texture);
            textureSwitcher.AddTrigger(textureSwitch.first, textureSwitch.second);
            eventSystem.Subscribe(&textureSwitcher, textureSwitch.first);
        }
    }
    auto& renderableMgr{Application::GetInstance().GetRenderSystem()->GetRenderableMgr()};
    for(int entityIndex = (int)firstEntity; entityIndex < (int)firstEntity + layer.entities.size(); ++entityIndex) {
        renderableMgr.Add(  layer.index,
                            (Entity)entityIndex,
                            *boundingBoxMgr.Get(entityIndex),
                            *spriteMgr.Get(entityIndex));
    }
}

void Scene::CreateMenuLabels(Layer& layer) {
    /*  A MenuLabel layer has:
            A list of ResourceTokens {ResourceID, path} for loading Fonts
            A list of Texts {std::string, FontParameters, ResourceID for Font to attach}
            A list of AlignLabels {Alignment}
    */
    LabelMgr& labelMgr{Application::GetInstance().GetRenderSystem()->GetLabelMgr()};
    AlignLabelMgr& alignLabelMgr{Application::GetInstance().GetRenderSystem()->GetAlignLabelMgr()};
    std::string buttonLayerName{"MenuButtonLayer"};
    const auto buttonLayerPtr{GetLayer(buttonLayerName)};
    if(!buttonLayerPtr) {
        return;
    }
    Layer& buttonLayer{*buttonLayerPtr};
    for(int index = 0; index < buttonLayer.entities.size(); ++index) {
        layer.entities.push_back(buttonLayer.entities.at(index));
    }
    Entity firstEntity{*layer.entities.begin()};
    int index = (int)firstEntity;
    for(const auto& textString : layer.textContents) {
        const auto& fontParam{layer.fontParameters.at(index - (int)firstEntity)};
        Resource* attachment(layer.textAttachments.at(index - (int)firstEntity));
        textMgr.Add((Entity)index);
        Text& text{*textMgr.Get((Entity)index)};
        text.Attach(attachment);
        text.SetFontParameters(fontParam);
        text.SetContents(textString);
        menu.options.push_back(MenuOption{(Entity)index, textString});
        BoundingBox& boundingBox{*boundingBoxMgr.Get((Entity)index)};
        labelMgr.Add((Entity)index, boundingBox, text);
        alignLabelMgr.Add((Entity)index, layer.labelAlignments.at(index - (int)firstEntity), *labelMgr.Get((Entity)index));
        index++;
    }
}

