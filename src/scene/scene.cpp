#include "../include/scene/scene.hpp"
#include "../include/core/app.hpp"

Scene::Scene(int scnIndex, EntityMgr& entMgr, ResourceMgr& resMgr):
    ISerializeable{},
    ILogMsgPublisher{},
    sceneIndex{scnIndex},
    entityMgr{entMgr},
    resourceMgr{resMgr},
    optionSelector{this} {

    auto& eventSystem{*Application::GetInstance().GetEventSystem()};
    for(int index = 0; index < Event::NumEventTypes; ++index) {
        eventSystem.Subscribe(&optionSelector, (Event::TypeID)index);
    }
}

void Scene::LoadFromJSON(const nlohmann::json& json) {
    SceneParser                         parser{json};
    layers =                            parser.LoadLayers();
    for(int index = 0; index < layers.size(); ++index) {
        Layer& layer{layers[index]};
        LoadResources(layer);
        if(layer.typeID == Layer::TypeID::Music) {
            PlayBackgroundMusic(layer);
        }
        else if(layer.typeID == Layer::TypeID::Decoration) {
            CreateDecorations(layer);
        }
        else if(layer.typeID == Layer::TypeID::MenuButton) {
            CreateMenuButtons(layer);
        }
        else if(layer.typeID == Layer::TypeID::MenuLabel) {
            CreateMenuLabels(layer);
        }
        else if(layer.typeID == Layer::TypeID::TitleBarLabel) {
            CreateTitleBarLabel(layer);
        }
    }
}

void Scene::LoadResources(Layer& layer) {
    for(const auto& resource : layer.resources) {
        switch(resource.typeID) {
            case Resource::TypeID::CompositeTexture: {
                resourceMgr.LoadTexture(    resource.id,
                                            resource.textureParams.size,
                                            resource.textureParams.sourceTextureIDs,
                                            resource.textureParams.destinations);
            } break;
            case Resource::TypeID::Font: {
                resourceMgr.LoadFont(resource.id, resource.path);

            } break;
            case Resource::TypeID::JSONDocument: {
                resourceMgr.LoadJSONDocument(resource.id, resource.path);
            } break;
            case Resource::TypeID::Music: {
                resourceMgr.LoadMusic(resource.id, resource.path);
            } break;
            case Resource::TypeID::RepeatingTexture: {
                if(     resource.textureParams.repeatHorizontal > 0
                    &&  resource.textureParams.repeatVertical == 0) {
                    resourceMgr.LoadTexture(    resource.id,
                                                resource.path,
                                                Orientation::Horizontal,
                                                resource.textureParams.repeatHorizontal,
                                                resource.textureParams.sourceRect);
                }
                else if(    resource.textureParams.repeatHorizontal == 0
                        &&  resource.textureParams.repeatVertical > 0) {
                    resourceMgr.LoadTexture(    resource.id,
                                                resource.path,
                                                Orientation::Vertical,
                                                resource.textureParams.repeatVertical,
                                                resource.textureParams.sourceRect);
                }
                else {
                    resourceMgr.LoadTexture(    resource.id,
                                                resource.path,
                                                sf::Vector2u{resource.textureParams.repeatHorizontal, resource.textureParams.repeatVertical},
                                                resource.textureParams.sourceRect);
                }
            } break;
            case Resource::TypeID::SimpleTexture: {
                resourceMgr.LoadTexture(resource.id, resource.path, resource.textureParams.sourceRect);
            } break;
            case Resource::TypeID::Sound: {
                resourceMgr.LoadSound(resource.id, resource.path);
            } break;
            case Resource::TypeID::TextFile: {
                resourceMgr.LoadTextFile(resource.id, resource.path);
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

MenuOptionSelector& Scene::GetOptionSelector() const {
    return const_cast<MenuOptionSelector&>(optionSelector);
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
    if(menu.options.empty()) {
        return;
    }
    if(!menu.selectedOption) {
        Entity      widgetUnderCursor{FindWidgetUnderCursor()};
        if((int)widgetUnderCursor > 0) {
            unsigned int menuIndex = (int)widgetUnderCursor - (int)menu.options.at(0).widget;
            menu.selectedOption = &menu.options.at(menuIndex);
        }
        else {
            return;
        }
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

Layer* Scene::GetLayer(std::string_view name) {
    for(auto iter = layers.begin(); iter != layers.end(); ++iter) {
        if(iter->name.compare(name) == 0) {
            return &(*iter);
        }
    }
    return nullptr;
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
        ResourceID attachmentID{layer.spriteAttachments.at(index - firstEntity)};
        Resource* attachment{resourceMgr.GetTexture(attachmentID)};
        sprite.Attach(attachment);
        index++;
    }
    auto& layerIndexMgr{Application::GetInstance().GetRenderSystem()->GetLayerIndexMgr()};
    auto& renderableMgr{Application::GetInstance().GetRenderSystem()->GetRenderableMgr()};
    for(int entityIndex = firstEntity; entityIndex < firstEntity + layer.entities.size(); ++entityIndex) {
        Sprite* sprite{spriteMgr.Get((Entity)entityIndex)};
        BoundingBox* boundingBox{boundingBoxMgr.Get(entityIndex)};
        if(sprite && boundingBox) {
            layerIndexMgr.Add((sceneIndex * 100) + layer.index, (Entity)entityIndex);
            renderableMgr.Add(  (Entity)entityIndex,
                                *layerIndexMgr.Get((Entity)entityIndex),
                                *boundingBoxMgr.Get((Entity)entityIndex),
                                *sprite);
            auto bounds{boundingBox->GetRect()};
            Resource*           attachedResource{sprite->GetAttachments().at(0)};
            Texture&            texture{*dynamic_cast<Texture*>(attachedResource)};
            if(     texture.GetSize().x < bounds.width
                ||  texture.GetSize().y < bounds.height) {
                scaleRenderableMgr.Add((Entity)entityIndex, *renderableMgr.Get(((Entity)entityIndex)));
            }
        }
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
        ResourceID attachmentID{layer.spriteAttachments.at(index - firstEntity)};
        Resource* attachment{resourceMgr.GetTexture(attachmentID)};
        sprite.Attach(attachment);
        index++;
    }
    for(const auto entity : layer.entities) {
        BoundingBox* boundingBoxPtr{boundingBoxMgr.Get(entity)};
        hoverableMgr.Add(entity, *boundingBoxPtr);
        inputSystem.Subscribe(hoverableMgr.Get(entity));
        textureSwitcherMgr.Add(entity, *spriteMgr.Get(entity));
        leftClickableMgr.Add(entity, *boundingBoxPtr);
        inputSystem.Subscribe(leftClickableMgr.Get(entity));
        for(const auto& textureSwitch : layer.textureSwitches) {
            ResourceID textureID{textureSwitch.second};
            Texture* texture{resourceMgr.GetTexture(textureID)};
            auto& textureSwitcher{*textureSwitcherMgr.Get(entity)};
            textureSwitcher.Attach(texture);
            textureSwitcher.AddTrigger(textureSwitch.first, textureSwitch.second);
            eventSystem.Subscribe(&textureSwitcher, textureSwitch.first);
        }
    }
    auto& layerIndexMgr{Application::GetInstance().GetRenderSystem()->GetLayerIndexMgr()};
    auto& renderableMgr{Application::GetInstance().GetRenderSystem()->GetRenderableMgr()};
    for(int entityIndex = (int)firstEntity; entityIndex < (int)firstEntity + layer.entities.size(); ++entityIndex) {
        Sprite* sprite{spriteMgr.Get((Entity)entityIndex)};
        BoundingBox* boundingBox{boundingBoxMgr.Get((Entity)entityIndex)};
        if(sprite && boundingBox) {
            layerIndexMgr.Add((sceneIndex * 100) + layer.index, (Entity)entityIndex);
            renderableMgr.Add(  (Entity)entityIndex,
                                *layerIndexMgr.Get((Entity)entityIndex),
                                *boundingBoxMgr.Get((Entity)entityIndex),
                                *sprite);
            auto bounds{boundingBox->GetRect()};
            Resource*           attachedResource{sprite->GetAttachments().at(0)};
            Texture&            texture{*dynamic_cast<Texture*>(attachedResource)};
            if(     texture.GetSize().x < bounds.width
                ||  texture.GetSize().y < bounds.height) {
                scaleRenderableMgr.Add((Entity)entityIndex, *renderableMgr.Get(((Entity)entityIndex)));
            }
        }
    }
    CreateSoundEffects(layer);
}

void Scene::CreateMenuLabels(Layer& layer) {
    /*  A MenuLabel layer has:
            A list of ResourceTokens {ResourceID, path} for loading Fonts
            A list of Texts {std::string, FontParameters, ResourceID for Font to attach}
            A list of Alignables {Alignment}
    */
    LabelMgr& labelMgr{Application::GetInstance().GetRenderSystem()->GetLabelMgr()};
    AlignableMgr& alignLabelMgr{Application::GetInstance().GetRenderSystem()->GetAlignableMgr()};
    std::string buttonLayerName{"MenuButtonLayer"};
    const auto buttonLayerPtr{GetLayer(buttonLayerName)};
    if(!buttonLayerPtr) {
        return;
    }
    Layer& buttonLayer{*buttonLayerPtr};
    std::vector<std::pair<Entity, Entity>>  labelButtonPairs; 
    for(int index = 0; index < buttonLayer.entities.size(); ++index) {
        Entity label{entityMgr.CreateEntity()};
        Entity button{buttonLayer.entities.at(index)};
        layer.entities.push_back(label);
        labelButtonPairs.push_back(std::make_pair(label, button));
    }
    Entity firstLabel{*layer.entities.begin()};
    int index = (int)firstLabel;
    for(const auto& labelButtonPair : labelButtonPairs) {
        Entity label{labelButtonPair.first};
        Entity button{labelButtonPair.second};
        const auto& fontParam{layer.fontParameters.at(index - (int)firstLabel)};
        ResourceID attachmentID(layer.textAttachments.at(index - (int)firstLabel));
        Resource* attachment{resourceMgr.GetFont(attachmentID)};
        textMgr.Add(label);
        Text& text{*textMgr.Get(label)};
        text.Attach(attachment);
        text.SetFontParameters(fontParam);
        std::string contents{layer.textContents.at(index - (int)firstLabel)};
        text.SetContents(contents);
        menu.options.push_back(MenuOption{label, contents});
        BoundingBox& buttonBoundingBox{*boundingBoxMgr.Get(button)};
        boundingBoxMgr.Add(label, buttonBoundingBox.GetRect());
        index++;
    }
    auto& layerIndexMgr{Application::GetInstance().GetRenderSystem()->GetLayerIndexMgr()};
    auto& renderableMgr{Application::GetInstance().GetRenderSystem()->GetRenderableMgr()};
    auto& alignableMgr{Application::GetInstance().GetRenderSystem()->GetAlignableMgr()};
    for(const auto& labelButtonPair : labelButtonPairs) {
        Entity label{labelButtonPair.first};
        Entity button{labelButtonPair.second};
        Text* text{textMgr.Get(label)};
        BoundingBox* boundingBox{boundingBoxMgr.Get(label)};
        if(text && boundingBox) {
            layerIndexMgr.Add((sceneIndex * 100) + layer.index, label);
            renderableMgr.Add(  label,
                                *layerIndexMgr.Get(label),
                                *boundingBoxMgr.Get(label),
                                *text);
            alignableMgr.Add(label, Alignment::Center, *boundingBoxMgr.Get(button));
        }
    }
}
void Scene::CreateTitleBarLabel(Layer& layer) {
    std::string                             titleBarLayerName{"TitleBarLayer"};
    const auto                              titleBarLayerPtr{GetLayer(titleBarLayerName)};
    if(!titleBarLayerPtr) {
        return;
    }
    Layer&                                  titleBarLayer{*titleBarLayerPtr};
    Entity                                  titleBar{titleBarLayer.entities.at(0)};

    Entity                                  titleBarLabel{entityMgr.CreateEntity()};
    layer.entities.push_back(titleBarLabel);
    textMgr.Add(titleBarLabel);
    Text&                                   text{*textMgr.Get(titleBarLabel)};
    ResourceID                              attachmentID{layer.textAttachments.at(0)};
    Resource*                               attachment{resourceMgr.GetFont(attachmentID)};
    text.Attach(attachment);
    text.SetFontParameters(layer.fontParameters.at(0));
    text.SetContents(layer.textContents.at(0));
    auto&                                   renderableMgr{Application::GetInstance().GetRenderSystem()->GetRenderableMgr()};
    auto&                                   alignableMgr{Application::GetInstance().GetRenderSystem()->GetAlignableMgr()};
    auto&                                   layerIndexMgr{Application::GetInstance().GetRenderSystem()->GetLayerIndexMgr()};
    layerIndexMgr.Add((sceneIndex * 100) + layer.index, titleBarLabel);
            renderableMgr.Add(  titleBarLabel,
                                *layerIndexMgr.Get(titleBarLabel),
                                *boundingBoxMgr.Get(titleBarLabel),
                                text);
    alignableMgr.Add(titleBarLabel, layer.labelAlignments.at(0), *boundingBoxMgr.Get(titleBar));
}

void Scene::CreateSoundEffects(Layer& layer) {
    SoundSystem&                    soundSystem{*Application::GetInstance().GetSoundSystem()};
    for(const auto& soundEffect : layer.soundEffects) {
        soundSystem.AddSoundEffect(SoundEffectToken{
            soundEffect.first,
            soundEffect.second
        });
    }
}

void Scene::PlayBackgroundMusic(Layer &layer) {
    MusicSystem&                    musicSystem{*Application::GetInstance().GetMusicSystem()};
    musicSystem.SetMusic(layer.backgroundMusicID);
    musicSystem.Play();
}

Entity Scene::FindWidgetUnderCursor() const {
    const auto&     boundingBoxList{boundingBoxMgr.GetList()};
    for(const auto& boundingBox : boundingBoxList) {
        if(IsInBounds((sf::Vector2<unsigned int>)sf::Mouse::getPosition(), boundingBox->GetRect())) {
            return boundingBox->GetOwner();
        }
    }
    return (Entity)-1;
}