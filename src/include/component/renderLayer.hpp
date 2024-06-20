#pragma once

#include "../core/component.hpp"

class RenderLayer : public Component {
public:
    RenderLayer(Entity cOwner, int layer);

    void                    LoadFromJSON(const nlohmann::json& json);
    nlohmann::json          SaveToJSON() const;

    int                     GetLayerIndex() const;
private:
    int                     layerIndex;
};

class RenderLayerMgr {
public:
    void                                                        Add(int layerIndex, Entity owner);

    void                                                        Remove(Entity owner);

    RenderLayer*                                                Get(Entity owner);

    std::vector<RenderLayer*>                                   GetList() const;
    
private:
    std::unordered_map<Entity, std::unique_ptr<RenderLayer>>    layerMap;
};