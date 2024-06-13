#pragma once

#include "boundingBox.hpp"
#include "sprite.hpp"

class Renderable: public Component {
public:
    Renderable(Entity cEntity, BoundingBox& boundsCmp, Sprite& spriteCmp);
    
    void                LoadFromJSON(const nlohmann::json& json);
    nlohmann::json      SaveToJSON() const;
    
    const BoundingBox&  GetBounds() const;
    Sprite&             GetSprite() const;

private:
    BoundingBox&        bounds;
    Sprite&             sprite;
};

class RenderableMgr {
public:
    void                                                        Add(Entity owner, BoundingBox& boundsCmp, Sprite& spriteCmp);

    void                                                        Remove(Entity owner);

    Renderable*                                                 Get(Entity owner);

    std::vector<Renderable*>                                    GetList() const;
    
private:
    std::unordered_map<Entity, std::unique_ptr<Renderable>>     renderableMap;
};