#pragma once

#include <memory>
#include <unordered_map>
#include <SFML/Graphics/Rect.hpp>
#include "../core/common.hpp"
#include "../core/component.hpp"

class BoundingBox: public Component {
public:
    BoundingBox(Entity cOwner);

    unsigned int                    GetTop() const;
    unsigned int                    GetLeft() const;
    unsigned int                    GetWidth() const;
    unsigned int                    GetHeight() const;
    const sf::Rect<unsigned int>&   GetRect() const;
    void                            SetRect(const sf::Rect<unsigned int>& rect);

    void                            LoadFromJSON(const nlohmann::json& json);
    
    nlohmann::json                  SaveToJSON() const;

private:
    sf::Rect<unsigned int>          boundsRect;
};

class BoundingBoxMgr {
public:
    void                            Add(Entity owner, const sf::Rect<unsigned int>& boundsRect = {0, 0, 0, 0});

    void                            Remove(Entity owner);

    BoundingBox*                    Get(Entity owner);

    std::vector<BoundingBox*>       GetList() const;

private:
    std::unordered_map<Entity, std::unique_ptr<BoundingBox>>        boundsMap;
};