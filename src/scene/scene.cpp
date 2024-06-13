#include "../include/scene/scene.hpp"
#include "../include/core/app.hpp"

Scene::Scene():
    ISerializeable{} {

}

void Scene::LoadFromJSON(const nlohmann::json& json) {
    EntityMgr& entityMgr{Application::GetInstance().GetEntityMgr()};

    const auto& findSprites{json.find("sprites")};
    const auto& findTexts{json.find("texts")};
    const auto& findBounds{json.find("bounds")};
    
    if(findSprites != json.end()) {
        for(auto spriteJSON : findSprites.value()) {
            Entity owner{entityMgr.CreateEntity()};
            this->buttons.push_back(owner);
            spriteMgr.Add(owner);
            Sprite* sprite{spriteMgr.Get(owner)};
            sprite->LoadFromJSON(spriteJSON);
        }
    }
    if(findTexts != json.end()) {
        int entityIndex = 0;
        for(auto textJSON : findTexts.value()) {
            Entity owner{this->buttons.at(entityIndex++)};
            textMgr.Add(owner);
            Text* text{textMgr.Get(owner)};
            text->LoadFromJSON(textJSON);
        }
    }
    if(findBounds != json.end()) {
        int entityIndex = 0;
        for(auto boundsJSON : findBounds.value()) {
            Entity owner{this->buttons.at(entityIndex++)};
            boundingBoxMgr.Add(owner);
            BoundingBox* bounds{boundingBoxMgr.Get(owner)};
            bounds->LoadFromJSON(boundsJSON);
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

const std::vector<Entity>& Scene::GetButtonList() const {
    return buttons;
}