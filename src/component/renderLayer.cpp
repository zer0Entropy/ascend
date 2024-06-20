#include "../include/component/renderLayer.hpp"

RenderLayer::RenderLayer(Entity cOwner, int layer):
    Component{Component::TypeID::RenderLayer, cOwner},
    layerIndex{layer} {

}

void RenderLayer::LoadFromJSON(const nlohmann::json& json) {

}

nlohmann::json RenderLayer::SaveToJSON() const {
    return nlohmann::json{};
}

int RenderLayer::GetLayerIndex() const {
    return layerIndex;
}

void RenderLayerMgr::Add(int layerIndex, Entity owner) {
    layerMap.insert(std::make_pair(owner, std::make_unique<RenderLayer>(owner, layerIndex)));
}

void RenderLayerMgr::Remove(Entity owner) {
    const auto& iter{layerMap.find(owner)};
    if(iter != layerMap.end()) {
        layerMap.erase(iter);
    }
}

RenderLayer* RenderLayerMgr::Get(Entity owner) {
    const auto& iter{layerMap.find(owner)};
    if(iter != layerMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<RenderLayer*> RenderLayerMgr::GetList() const {
    std::vector<RenderLayer*>   layerList;
    for(const auto& renderLayer : layerMap) {
        layerList.push_back(renderLayer.second.get());
    }
    return layerList;
}