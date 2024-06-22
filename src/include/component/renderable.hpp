#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include "boundingBox.hpp"
#include "layerIndex.hpp"

class Sprite;
class Text;

class Renderable: public Component {
public:
    Renderable(Entity cEntity, const LayerIndex& layerCmp, const BoundingBox& boundsCmp, Sprite& spriteCmp);
    Renderable(Entity cEntity, const LayerIndex& layerCmp, const BoundingBox& boundsCmp, Text& textCmp);
    
    void                            LoadFromJSON(const nlohmann::json& json);
    nlohmann::json                  SaveToJSON() const;
    
    const LayerIndex&               GetLayerIndex() const;
    const BoundingBox&              GetBounds() const;
    Sprite*                         GetSprite() const;
    Text*                           GetText() const;

private:
    const LayerIndex&               layer;
    const BoundingBox&              bounds;
    Sprite*                         sprite;
    Text*                           text;
};

class RenderableMgr {
public:
    void                                                        Add(    Entity owner,
                                                                        const LayerIndex& layerCmp,
                                                                        const BoundingBox& boundsCmp,
                                                                        Sprite& spriteCmp);

    void                                                        Add(    Entity owner,
                                                                        const LayerIndex& layerCmp,
                                                                        const BoundingBox& boundsCmp,
                                                                        Text& textCmp);

    void                                                        Remove(Entity owner);

    Renderable*                                                 Get(Entity owner);

    std::vector<Renderable*>                                    GetList() const;
    
private:
    void                                                        AddLayer(int index);
    void                                                        RemoveLayer(int index);
    std::vector<Renderable*>*                                   GetLayer(int index) const;

    std::vector<std::pair<int, std::vector<Renderable*>>>       layers;
    std::unordered_map<Entity, std::unique_ptr<Renderable>>     renderableMap;
};