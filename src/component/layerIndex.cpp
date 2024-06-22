#include "../include/component/layerIndex.hpp"

LayerIndex::LayerIndex(Entity cOwner, int layerIndex):
    Component{Component::TypeID::LayerIndex, cOwner},
    index{layerIndex} {

}

void LayerIndex::LoadFromJSON(const nlohmann::json& json) {

}

nlohmann::json LayerIndex::SaveToJSON() const {
    return nlohmann::json{};
}

int LayerIndex::GetIndex() const {
    return index;
}

void LayerIndexMgr::Add(int layerIndex, Entity owner) {
    layerMap.insert(std::make_pair(owner, std::make_unique<LayerIndex>(owner, layerIndex)));
}

void LayerIndexMgr::Remove(Entity owner) {
    const auto& iter{layerMap.find(owner)};
    if(iter != layerMap.end()) {
        layerMap.erase(iter);
    }
}

LayerIndex* LayerIndexMgr::Get(Entity owner) {
    const auto& iter{layerMap.find(owner)};
    if(iter != layerMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<LayerIndex*> LayerIndexMgr::GetList() const {
    std::vector<LayerIndex*>   layerList;
    for(const auto& renderLayer : layerMap) {
        layerList.push_back(renderLayer.second.get());
    }
    return layerList;
}