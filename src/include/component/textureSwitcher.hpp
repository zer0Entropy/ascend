#pragma once

#include "sprite.hpp"
#include "../core/event.hpp"
#include "../interface/attachment.hpp"
#include "../interface/subscriber.hpp"

class TextureSwitcher: public Component, public ICanHaveAttachments, public IEventSubscriber {
public:
    TextureSwitcher(Entity cEntity, Sprite& spriteCmp);

    void                            LoadFromJSON(const nlohmann::json& json) override;
    nlohmann::json                  SaveToJSON() const override;

    void                            Attach(Resource* resource) override;
    void                            Detatch(const ResourceID& resourceID) override;

    void                            HandleEvent(const Event& event) override;
    
    ResourceID                      GetTrigger(Event::TypeID triggerID) const;
    void                            AddTrigger(Event::TypeID triggerID, ResourceID textureID);
    void                            RemoveTrigger(Event::TypeID triggerID);

private:
    Sprite&                                             sprite;
    std::unordered_map<Event::TypeID, ResourceID>       triggerMap;
};

class TextureSwitcherMgr {
public:
    void                            Add(Entity owner, Sprite& spriteCmp);

    void                            Remove(Entity owner);

    TextureSwitcher*                Get(Entity owner);

    std::vector<TextureSwitcher*>   GetList() const;

private:
    std::unordered_map<Entity, std::unique_ptr<TextureSwitcher>>        textureSwitcherMap;
};