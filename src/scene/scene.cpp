#include "../include/scene/scene.hpp"
#include "../include/core/app.hpp"

Scene::Scene(EntityMgr& entMgr, ResourceMgr& resMgr):
    ISerializeable{},
    ILogMsgPublisher{},
    entityMgr{entMgr},
    resourceMgr{resMgr} {

}

void Scene::LoadFromJSON(const nlohmann::json& json) {
    const auto& findEntities{json.find("entities")};
    if(findEntities != json.end()) {
        int numEntities{findEntities.value()};
        for(int index = 0; index < numEntities; ++index) {
            entityList.push_back(entityMgr.CreateEntity());
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
            LoadSprite(entityList.at(index++), spriteJSON);
        }
    }

    const auto& findTexts{json.find("texts")};
    if(findTexts != json.end()) {
        int index = 0;
        for(auto textJSON : findTexts.value()) {
            LoadText(entityList.at(index++), textJSON);
        }
    }
    
    const auto& findBounds{json.find("bounds")};
    if(findBounds != json.end()) {
        int index = 0;
        for(auto boundsJSON : findBounds.value()) {
            LoadBoundingBox(entityList.at(index++), boundsJSON);
        }
    }

    auto& renderableMgr{Application::GetInstance().GetRenderSystem()->GetRenderableMgr()};
    auto& labelMgr{Application::GetInstance().GetRenderSystem()->GetLabelMgr()};
    for(const auto& entity : entityList) {
        auto    bounds{boundingBoxMgr.Get(entity)};
        auto    sprite{spriteMgr.Get(entity)};
        auto    text{textMgr.Get(entity)};
        if(bounds && sprite) {
            renderableMgr.Add(entity, *bounds, *sprite);
        }
        if(bounds && text) {
            labelMgr.Add(entity, *bounds, *text);
        }
    }

    auto& alignLabelMgr{Application::GetInstance().GetRenderSystem()->GetAlignLabelMgr()};
    const auto& findAlignLabels{json.find("alignLabels")};
    if(findAlignLabels != json.end()) {
        int index = 0;
        for(auto alignLabelJSON : findAlignLabels.value()) {
            Entity      owner{entityList.at(index++)};
            Alignment   alignment{LoadAlignLabel(alignLabelJSON)};
            Label*      label{labelMgr.Get(owner)};
            alignLabelMgr.Add(owner, alignment, *label);
        }
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

const std::vector<Entity>& Scene::GetEntityList() const {
    return entityList;
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
