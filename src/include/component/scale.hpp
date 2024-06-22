#pragma once

#include "renderable.hpp"

class ScaleRenderable: public Component {
public:
    ScaleRenderable(Entity cEntity, const Renderable& renderableCmp);

    void                            LoadFromJSON(const nlohmann::json& json) override;
    
    nlohmann::json                  SaveToJSON() const override;

    const sf::Vector2f&             GetScalingFactor() const;
private:
    sf::Vector2f                    scalingFactor;
    const Renderable&               renderable;
};

class ScaleRenderableMgr {
public:
    void                            Add(Entity owner, const Renderable& renderableCmp);

    void                            Remove(Entity owner);

    ScaleRenderable*                Get(Entity owner);

    std::vector<ScaleRenderable*>   GetList() const;
private:
    std::unordered_map<Entity, std::unique_ptr<ScaleRenderable>>        scaleMap;
};