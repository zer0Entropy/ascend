#pragma once

#include <array>
#include <queue>
#include "../core/entity.hpp"
#include "../interface/subscriber.hpp"
#include "../interface/serialize.hpp"
#include "../interface/system.hpp"

class Event: public ISerializeable {
public:
    enum class TypeID {
        CursorHoveringStarted,
        CursorHoveringStopped
    };
    constexpr static int NumEventTypes = 2;

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

    void                                Update();
    ISystem::SystemID                   GetSystemID() const;
    void                                Enqueue(const Event& event);
    void                                Subscribe(IEventSubscriber* subscriber, Event::TypeID eventType);
    void                                Unsubscribe(IEventSubscriber* subscriber, Event::TypeID eventType);
private:
    std::queue<Event>                                                   queue;
    std::array<std::vector<IEventSubscriber*>, Event::NumEventTypes>    subscribers;
};