#pragma once

#include "../core/common.hpp"
#include "../core/component.hpp"

class BoundingBox;

class Alignable: public Component {
public:
    Alignable(Entity cEntity, Alignment align, const BoundingBox& alignBounds);

    void                            LoadFromJSON(const nlohmann::json& json);
    nlohmann::json                  SaveToJSON() const;

    Alignment                       GetAlignment() const;
    void                            SetAlignment(Alignment align);
    const BoundingBox&              GetTargetBoundingBox() const;

private:
    Alignment                       alignment;
    const BoundingBox&              targetBoundingBox;
};

class AlignableMgr {
public:
    void                            Add(Entity owner, Alignment align, const BoundingBox& bounds);

    void                            Remove(Entity owner);

    Alignable*                      Get(Entity owner);

    std::vector<Alignable*>         GetList() const;

private:
    std::unordered_map<Entity, std::unique_ptr<Alignable>>             alignLabelMap;
};