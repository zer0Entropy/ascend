#pragma once

#include "../core/component.hpp"

class LayerIndex : public Component {
public:
    LayerIndex(Entity cOwner, int layerIndex);

    void                        LoadFromJSON(const nlohmann::json& json);
    nlohmann::json              SaveToJSON() const;

    int                         GetIndex() const;
private:
    int                         index;
};

class LayerIndexMgr {
public:
    void                        Add(int layerIndex, Entity owner);

    void                        Remove(Entity owner);

    LayerIndex*                 Get(Entity owner);

    std::vector<LayerIndex*>    GetList() const;
    
private:
    std::unordered_map<Entity, std::unique_ptr<LayerIndex>>    layerMap;
};