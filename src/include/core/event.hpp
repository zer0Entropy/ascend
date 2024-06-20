#pragma once

#include <array>
#include <queue>
#include <string_view>
#include "../core/entity.hpp"
#include "../interface/publisher.hpp"
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
        ButtonPressCompleted,
        ButtonPressAborted,
        NewGameStarted,
        LoadGameStarted,
        OptionsStarted,
        QuitGameStarted
    };
    constexpr static int NumEventTypes = 9;

    static constexpr std::array<std::string_view, NumEventTypes> TypeNames{
        "CursorHoveringStarted"sv,
        "CursorHoveringStopped"sv,
        "ButtonPressStarted"sv,
        "ButtonPressCompleted"sv,
        "ButtonPressAborted"sv,
        "NewGameStarted"sv,
        "LoadGameStarted"sv,
        "OptionsStarted"sv,
        "QuitGameStarted"sv
    };

    static constexpr TypeID GetTypeID(std::string_view name) {
        TypeID              output{TypeID::QuitGameStarted};
        for(unsigned int index = 0; index < NumEventTypes; ++index) {
            if(name.compare(TypeNames[index]) == 0) {
                output = (TypeID)index;
                break;
            }
        }
        return output;
    }

    Event(TypeID type, Entity targetEnt);

    void                    LoadFromJSON(const nlohmann::json& json);
    nlohmann::json          SaveToJSON() const;

    TypeID                  GetTypeID() const;
    Entity                  GetTarget() const;

private:
    TypeID                  typeID;
    Entity                  target;
};

class EventSystem: public ISystem, public ILogMsgPublisher {
public:
    EventSystem();

    void                                Update() override;
    ISystem::SystemID                   GetSystemID() const override;

    void                                Enqueue(const Event& event);
    void                                Subscribe(IEventSubscriber* subscriber, Event::TypeID eventType);
    void                                Unsubscribe(IEventSubscriber* subscriber, Event::TypeID eventType);
    void                                UnsubscribeAll(IEventSubscriber* subscriber);
private:
    std::queue<Event>                                                   queue;
    std::array<std::vector<IEventSubscriber*>, Event::NumEventTypes>    subscribers;
};