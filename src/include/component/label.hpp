#pragma once

#include "boundingBox.hpp"
#include "text.hpp"

class Label: public Component {
public:
    Label(Entity cEntity, const BoundingBox& boundsCmp, Text& textCmp);

    void                    LoadFromJSON(const nlohmann::json& json);
    nlohmann::json          SaveToJSON() const;

    const BoundingBox&      GetBounds() const;
    Text&                   GetText() const;

private:
    const BoundingBox&      bounds;
    Text&                   text;
};

class LabelMgr {
public:
    void                                                        Add(Entity owner, const BoundingBox& boundsCmp, Text& textCmp);

    void                                                        Remove(Entity owner);

    Label*                                                      Get(Entity owner);

    std::vector<Label*>                                         GetList() const;
    
private:
    std::unordered_map<Entity, std::unique_ptr<Label>>          labelMap;
};