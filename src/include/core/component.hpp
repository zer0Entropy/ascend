#pragma once

#include "entity.hpp"
#include "../interface/serialize.hpp"

class Component: public ISerializeable {
public:
    enum class TypeID {
        BoundingBox,
        Sprite,
        Text,
        Renderable,
        ScaleRenderable,
        Label,
        AlignLabel,
        Hoverable,
        TextureSwitcher,
        LeftClickable
    };

    Component(TypeID cType, Entity cOwner):
        typeID{cType}, owner{cOwner} {}

    auto    GetTypeID() const   ->  TypeID  {return typeID;}
    [[nodiscard]]

    auto    GetOwner() const    ->  Entity  {return owner;}

    bool    IsEnabled() const {return enabled;}
    void    ToggleEnabled() {enabled = !enabled;}
    void    SetEnabled(bool enable) {enabled = enable;}
private:
    TypeID  typeID;
    Entity  owner;
    bool    enabled;
};
