#include "../include/component/renderable.hpp"

Renderable::Renderable(Entity cEntity, BoundingBox& boundsCmp, Sprite& spriteCmp):
    Component{Component::TypeID::Renderable, cEntity},
    bounds{boundsCmp},
    sprite{spriteCmp} {

}

void Renderable::LoadFromJSON(const nlohmann::json& json) {

}

nlohmann::json Renderable::SaveToJSON() const {
    return nlohmann::json{};
}

const BoundingBox& Renderable::GetBounds() const {
    return bounds;
}

Sprite& Renderable::GetSprite() const {
    return sprite;
}

void RenderableMgr::Add(Entity owner, BoundingBox& boundsCmp, Sprite& spriteCmp) {
    renderableMap.insert(std::make_pair(owner, std::make_unique<Renderable>(owner, boundsCmp, spriteCmp)));
}

void RenderableMgr::Remove(Entity owner) {
    auto iter{renderableMap.find(owner)};
    if(iter != renderableMap.end()) {
        iter->second.reset(nullptr);
        renderableMap.erase(iter);
    }
}

Renderable* RenderableMgr::Get(Entity owner) {
    auto iter{renderableMap.find(owner)};
    if(iter != renderableMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<Renderable*> RenderableMgr::GetList() const {
    std::vector<Renderable*> renderableList;
    for(const auto& iter : renderableMap) {
        renderableList.push_back(iter.second.get());
    }
    return renderableList;
}