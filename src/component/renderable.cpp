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

void RenderableMgr::Add(unsigned int layerIndex, Entity owner, BoundingBox& boundsCmp, Sprite& spriteCmp) {
    renderableMap.insert(std::make_pair(owner, std::make_unique<Renderable>(owner, boundsCmp, spriteCmp)));
    while(layerIndex >= layers.size()) {
        layers.push_back(RenderLayer{});
    }
    layers[layerIndex].push_back(Get(owner));
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
    for(const auto& renderLayer : layers) {
        for(auto renderable : renderLayer) {
            renderableList.push_back(renderable);
        }
    }
    return renderableList;
}