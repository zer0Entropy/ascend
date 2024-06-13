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
        Label,
        AlignLabel
    };

    Component(TypeID cType, Entity cOwner):
        typeID{cType}, owner{cOwner} {}

    auto    GetTypeID() const   ->  TypeID  {return typeID;}
    [[nodiscard]]

    auto    GetOwner() const    ->  Entity  {return owner;}

private:
    TypeID  typeID;
    Entity  owner;
};
