#include "../include/component/renderable.hpp"

Renderable::Renderable(Entity cEntity, const LayerIndex& layerCmp, const BoundingBox& boundsCmp, Sprite& spriteCmp):
    Component{Component::TypeID::Renderable, cEntity},
    layer{layerCmp},
    bounds{boundsCmp},
    sprite{&spriteCmp},
    text{nullptr} {

}

Renderable::Renderable(Entity cEntity, const LayerIndex& layerCmp, const BoundingBox& boundsCmp, Text& textCmp):
    Component{Component::TypeID::Renderable, cEntity},
    layer{layerCmp},
    bounds{boundsCmp},
    sprite{nullptr},
    text{&textCmp} {

}

void Renderable::LoadFromJSON(const nlohmann::json& json) {

}

nlohmann::json Renderable::SaveToJSON() const {
    return nlohmann::json{};
}

const LayerIndex& Renderable::GetLayerIndex() const {
    return layer;
}

const BoundingBox& Renderable::GetBounds() const {
    return bounds;
}

Sprite* Renderable::GetSprite() const {
    return sprite;
}

Text* Renderable::GetText() const {
    return text;
}

void RenderableMgr::Add(Entity owner, const LayerIndex& layerCmp, const BoundingBox& boundsCmp, Sprite& spriteCmp) {
    renderableMap.insert(std::make_pair(owner, std::make_unique<Renderable>(owner, layerCmp, boundsCmp, spriteCmp)));
    auto layer{GetLayer(layerCmp.GetIndex())};
    if(!layer) {
        AddLayer(layerCmp.GetIndex());
        layer = GetLayer(layerCmp.GetIndex());
    }
    layer->push_back(this->Get(owner));
}

void RenderableMgr::Add(Entity owner, const LayerIndex& layerCmp, const BoundingBox& boundsCmp, Text& textCmp) {
    renderableMap.insert(std::make_pair(owner, std::make_unique<Renderable>(owner, layerCmp, boundsCmp, textCmp)));
    auto layer{GetLayer(layerCmp.GetIndex())};
    if(!layer) {
        AddLayer(layerCmp.GetIndex());
        layer = GetLayer(layerCmp.GetIndex());
    }
    layer->push_back(this->Get(owner));
}

void RenderableMgr::Remove(Entity owner) {
    auto iter{renderableMap.find(owner)};
    if(iter != renderableMap.end()) {
        Renderable* renderable{iter->second.get()};
        const LayerIndex& layerIndex{renderable->GetLayerIndex()};
        auto& layer{*GetLayer(layerIndex.GetIndex())};
        for(auto layerIter = layer.begin(); layerIter != layer.end(); ++layerIter) {
            if((*layerIter)->GetOwner() == owner) {
                layer.erase(layerIter);
                if(layer.empty()) {
                    RemoveLayer(layerIndex.GetIndex());
                }
                break;
            }
        }
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
    for(const auto& layer : layers) {
        for(auto renderable : layer.second) {
            renderableList.push_back(renderable);
        }
    }
    return renderableList;
}

void RenderableMgr::AddLayer(int index) {
    for(auto iter = layers.begin(); iter != layers.end(); ++iter) {
        if((*iter).first > index) {
            layers.insert(iter, std::make_pair(index, std::vector<Renderable*>{}));
            return;
        }
    }
    layers.push_back(std::make_pair(index, std::vector<Renderable*>{}));
}

void RenderableMgr::RemoveLayer(int index) {
    for(auto iter = layers.begin(); iter != layers.end(); ++iter) {
        if((*iter).first == index) {
            (*iter).second.clear();
            layers.erase(iter);
            break;
        }
    }
}

std::vector<Renderable*>* RenderableMgr::GetLayer(int index) const {
    for(const auto& layer : layers) {
        if(layer.first == index) {
            return const_cast<std::vector<Renderable*>*>(&layer.second);
        }
    }
    return nullptr;
}