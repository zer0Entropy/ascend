#pragma once

#include "boundingBox.hpp"
#include "../interface/listener.hpp"

class EventSystem;

class LeftClickable: public Component, public IInputListener {
public:
    LeftClickable(Entity cEntity, const BoundingBox& boundsCmp, EventSystem& eventSys);

    void                    LoadFromJSON(const nlohmann::json& json) override;
    nlohmann::json          SaveToJSON() const override;

    bool                    ReceiveInput(const sf::Event& event) override;

    bool                    IsPressedDown() const;
    void                    SetPressedDown(bool pressedDown);
private:
    EventSystem&            eventSystem;
    const BoundingBox&      boundingBox;
    bool                    isPressedDown;
};

class LeftClickableMgr {
public:
    void                            Add(Entity owner, BoundingBox& boundsCmp);

    void                            Remove(Entity owner);

    LeftClickable*                  Get(Entity owner);

    std::vector<LeftClickable*>     GetList() const;
private:
    std::unordered_map<Entity, std::unique_ptr<LeftClickable>>  lClickMap;
};