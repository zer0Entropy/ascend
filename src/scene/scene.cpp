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
    const auto& findMenu{json.find("menu")};
    if(findMenu != json.end()) {
        const auto& menuJSON{findMenu.value()};
        const auto& findOptions{menuJSON.find("options")};
        if(findOptions != menuJSON.end()) {
            const auto& optionsVec{findOptions.value()};
            for(auto optionJSON : optionsVec) {
                MenuOption option;
                optionJSON.get_to(option.name);
                option.widget = entityMgr.CreateEntity();
                menu.options.push_back(option);
            }
        }
    }

    const auto& findResources{json.find("resources")};
    if(findResources != json.end()) {
        const auto& resourceJSON{findResources.value()};
        LoadResourceList(resourceJSON);
    }

    const auto& findSprites{json.find("sprites")};
    if(findSprites != json.end()) {
        int index = 0;
        for(auto spriteJSON : findSprites.value()) {
            LoadSprite(menu.options.at(index++).widget, spriteJSON);
        }
    }

    const auto& findTexts{json.find("texts")};
    if(findTexts != json.end()) {
        int index = 0;
        for(auto textJSON : findTexts.value()) {
            LoadText(menu.options.at(index++).widget, textJSON);
        }
    }
    
    const auto& findBounds{json.find("bounds")};
    if(findBounds != json.end()) {
        int index = 0;
        for(auto boundsJSON : findBounds.value()) {
            LoadBoundingBox(menu.options.at(index++).widget, boundsJSON);
        }
    }

    auto& inputSystem{*Application::GetInstance().GetInputSystem()};
    auto& renderableMgr{Application::GetInstance().GetRenderSystem()->GetRenderableMgr()};
    auto& labelMgr{Application::GetInstance().GetRenderSystem()->GetLabelMgr()};
    for(int index = 0; index < menu.options.size(); ++index) {
        Entity  entity{menu.options.at(index).widget};
        auto    bounds{boundingBoxMgr.Get(entity)};
        auto    sprite{spriteMgr.Get(entity)};
        auto    text{textMgr.Get(entity)};
        if(bounds && sprite) {
            renderableMgr.Add(entity, *bounds, *sprite);
        }
        if(bounds && text) {
            labelMgr.Add(entity, *bounds, *text);
        }
        if(bounds) {
            hoverableMgr.Add(entity, *bounds);
            auto hoverable{hoverableMgr.Get(entity)};
            inputSystem.Subscribe(hoverable);

            leftClickableMgr.Add(entity, *bounds);
            auto leftClickable{leftClickableMgr.Get(entity)};
            inputSystem.Subscribe(leftClickable);
        }
        if(sprite) {
            textureSwitcherMgr.Add(entity, *sprite);
        }
    }

    auto& alignLabelMgr{Application::GetInstance().GetRenderSystem()->GetAlignLabelMgr()};
    const auto& findAlignLabels{json.find("alignLabels")};
    if(findAlignLabels != json.end()) {
        int index = 0;
        for(auto alignLabelJSON : findAlignLabels.value()) {
            Entity              owner{menu.options.at(index++).widget};
            Alignment           alignment{LoadAlignLabel(alignLabelJSON)};
            Label*              label{labelMgr.Get(owner)};
            alignLabelMgr.Add(owner, alignment, *label);
        }
    }

    const auto& findTextureSwitches{json.find("textureSwitches")};
    if(findTextureSwitches != json.end()) {
        LoadTextureSwitches(findTextureSwitches.value());
    }
}

nlohmann::json Scene::SaveToJSON() const {
    nlohmann::json spriteOutput;
    const auto spriteList{spriteMgr.GetList()};
    for(const auto& sprite : spriteList) {
        spriteOutput += sprite->SaveToJSON();
    }
    nlohmann::json textOutput;
    const auto textList{textMgr.GetList()};
    for(const auto& text : textList) {
        textOutput += text->SaveToJSON();
    }
    nlohmann::json boundsOutput;
    const auto boundsList{boundingBoxMgr.GetList()};
    for(const auto& bounds : boundsList) {
        boundsOutput += bounds->SaveToJSON();
    }
    const nlohmann::json output{
            {"sprites",
                spriteOutput
            },
            {"texts",
                textOutput
            },
            {"bounds",
                boundsOutput
            }
    };
    return output;
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

const std::vector<ResourceToken>& Scene::GetFontList() const {
    return fontList;
}

const std::vector<ResourceToken>& Scene::GetTextureList() const {
    return textureList;
}

void Scene::LoadResourceList(const nlohmann::json& json) {
    const auto& findFonts{json.find("fonts")};
    const auto& findTextures{json.find("textures")};
    if(findFonts != json.end()) {
        for(auto fontJSON : findFonts.value()) {
            const auto& findID{fontJSON.find("id")};
            const auto& findPath{fontJSON.find("path")};
            if(     findID != fontJSON.end()
                &&  findPath != fontJSON.end()) {
                const ResourceToken font{
                    findID.value().template get<std::string>(),
                    findPath.value().template get<std::string>()
                };
                fontList.push_back(font);
            }
        }
        for(const auto& font : fontList) {
            resourceMgr.LoadFont(font.id, font.path);
        }
    }
    if(findTextures != json.end()) {
        for(auto textureJSON : findTextures.value()) {
            const auto& findID{textureJSON.find("id")};
            const auto& findPath{textureJSON.find("path")};
            if(     findID != textureJSON.end()
                &&  findPath != textureJSON.end()) {
                const ResourceToken texture{
                    findID.value().template get<std::string>(),
                    findPath.value().template get<std::string>()
                };
                textureList.push_back(texture);
            }
        }
        for(const auto& texture : textureList) {
            resourceMgr.LoadTexture(texture.id, texture.path);
        }
    }
}

void Scene::LoadSprite(Entity owner, const nlohmann::json& json) {
    spriteMgr.Add(owner);
    Sprite* sprite{spriteMgr.Get(owner)};
    sprite->LoadFromJSON(json);
}

void Scene::LoadText(Entity owner, const nlohmann::json& json) {
    textMgr.Add(owner);
    Text* text{textMgr.Get(owner)};
    text->LoadFromJSON(json);
}

void Scene::LoadBoundingBox(Entity owner, const nlohmann::json& json) {
    boundingBoxMgr.Add(owner);
    BoundingBox* bounds{boundingBoxMgr.Get(owner)};
    bounds->LoadFromJSON(json);
}

Alignment Scene::LoadAlignLabel(const nlohmann::json& json) const {
    Alignment alignment{Alignment::Left};
    const auto& findAlignment{json.find("alignment")};
    const std::string alignName{findAlignment.value().template get<std::string>()};
    if(alignName.compare(AlignmentNames.at((int)Alignment::Left)) == 0) {
        alignment = Alignment::Left;
    }
    else if(alignName.compare(AlignmentNames.at((int)Alignment::Center)) == 0) {
        alignment = Alignment::Center;
    }
    else if(alignName.compare(AlignmentNames.at((int)Alignment::Right)) == 0) {
        alignment = Alignment::Right;
    }
    return alignment;
}

void Scene::LoadTextureSwitches(const nlohmann::json& json) {
    auto& eventSystem{*Application::GetInstance().GetEventSystem()};
    for(int index = 0; index < menu.options.size(); ++index) {
        Entity                              owner{menu.options.at(index).widget};
        auto                                sprite{spriteMgr.Get(owner)};
        if(sprite) {
            textureSwitcherMgr.Add(owner, *sprite);
            auto&                           textureSwitcher{*textureSwitcherMgr.Get(owner)};
            for(auto textureSwitchJSON : json) {
                const auto&                 findTriggerEvent{textureSwitchJSON.find("triggerEvent")};
                const auto&                 findTextureID{textureSwitchJSON.find("textureID")};
                Event::TypeID               triggerEvent;
                ResourceID                  textureID;
                if(findTriggerEvent != textureSwitchJSON.end()) {
                    std::string     eventName{findTriggerEvent.value().template get<std::string>()};
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
                }
                if(findTextureID != textureSwitchJSON.end()) {
                    findTextureID.value().get_to(textureID);
                    if(!textureSwitcher.Contains(textureID)) {
                        Texture*        texture{resourceMgr.GetTexture(textureID)};
                        textureSwitcher.Attach(texture);
                    }
                }
                if(     findTriggerEvent != textureSwitchJSON.end()
                    &&  findTextureID != textureSwitchJSON.end()) {
                    textureSwitcher.AddTrigger(triggerEvent, textureID);
                    eventSystem.Subscribe(&textureSwitcher, triggerEvent);
                }
            }
        }
    }    
}