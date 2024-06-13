#pragma once

#include "label.hpp"

class AlignLabel: public Component {
public:
    AlignLabel(Entity cEntity, Alignment align, Label& labelCmp);

    void                LoadFromJSON(const nlohmann::json& json);
    nlohmann::json      SaveToJSON() const;

    Alignment           GetAlignment() const;
    void                SetAlignment(Alignment align);
    Label&              GetLabel() const;

private:
    Alignment           alignment;
    Label&              label;
};

class AlignLabelMgr {
public:
    void                            Add(Entity owner, Alignment align, Label& labelCmp);

    void                            Remove(Entity owner);

    AlignLabel*                     Get(Entity owner);

    std::vector<AlignLabel*>        GetList() const;

private:
    std::unordered_map<Entity, std::unique_ptr<AlignLabel>>             alignLabelMap;
};