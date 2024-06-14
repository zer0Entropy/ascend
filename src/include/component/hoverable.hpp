#pragma once

#include "boundingBox.hpp"
#include "../interface/listener.hpp"

class EventSystem;

class Hoverable: public Component, public IInputListener {
public:
    Hoverable(Entity cEntity, const BoundingBox& boundsCmp, EventSystem& eventSys);

    void                    LoadFromJSON(const nlohmann::json& json) override;
    nlohmann::json          SaveToJSON() const override;

    bool                    ReceiveInput(const sf::Event& event) override;

    enum class HoverStatus {
        On,
        Off
    };
    HoverStatus             GetHoverStatus() const;

private:
    EventSystem&            eventSystem;
    const BoundingBox&      boundingBox;
    HoverStatus             status;
};

class HoverableMgr {
public:
    void                            Add(Entity owner, BoundingBox& boundsCmp);

    void                            Remove(Entity owner);

    Hoverable*                      Get(Entity owner);

    std::vector<Hoverable*>         GetList() const;
private:
    std::unordered_map<Entity, std::unique_ptr<Hoverable>>  hoverableMap;
};