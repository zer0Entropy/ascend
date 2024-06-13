#pragma once

#include <memory>
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include "../core/component.hpp"
#include "../interface/attachment.hpp"
#include "../resource/texture.hpp"

class Sprite: public Component, public ICanHaveAttachments {
public:
    Sprite(Entity cEntity);
    
    sf::Sprite&                     GetSprite() const;

    void                            LoadFromJSON(const nlohmann::json& json) override;
    
    nlohmann::json                  SaveToJSON() const override;

    void                            Attach(Resource* resource) override;

    void                            Detatch(const ResourceID& resourceID) override;

private:
    sf::Sprite                      sprite;
};

class SpriteMgr {
public:
    void                            Add(Entity owner);

    void                            Remove(Entity owner);

    Sprite*                         Get(Entity owner);

    std::vector<Sprite*>            GetList() const;
    
private:
    std::unordered_map<Entity, std::unique_ptr<Sprite>>     spriteMap;
};