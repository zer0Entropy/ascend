#pragma once

#include <array>
#include <queue>
#include <string_view>
#include "../core/entity.hpp"
#include "../interface/subscriber.hpp"
#include "../interface/serialize.hpp"
#include "../interface/system.hpp"

using namespace std::literals::string_view_literals;

class Event: public ISerializeable {
public:
    enum class TypeID {
        CursorHoveringStarted,
        CursorHoveringStopped,
        ButtonPressStarted,
        ButtonPressStopped
    };
    constexpr static int NumEventTypes = 4;

    static constexpr std::array<std::string_view, NumEventTypes> TypeNames{
        "CursorHoveringStarted"sv,
        "CursorHoveringStopped"sv,
        "ButtonPressStarted"sv,
        "ButtonPressStopped"sv
    };

    Event(TypeID type, Entity targetEnt);

    void                    LoadFromJSON(const nlohmann::json& json);
    nlohmann::json          SaveToJSON() const;

    TypeID                  GetTypeID() const;
    Entity                  GetTarget() const;

private:
    TypeID                  typeID;
    Entity                  target;
};

class EventSystem: public ISystem {
public:
    EventSystem();

    void                                Update() override;
    ISystem::SystemID                   GetSystemID() const override;

    void                                Enqueue(const Event& event);
    void                                Subscribe(IEventSubscriber* subscriber, Event::TypeID eventType);
    void                                Unsubscribe(IEventSubscriber* subscriber, Event::TypeID eventType);
private:
    std::queue<Event>                                                   queue;
    std::array<std::vector<IEventSubscriber*>, Event::NumEventTypes>    subscribers;
};