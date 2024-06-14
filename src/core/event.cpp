#include "../include/core/event.hpp"

Event::Event(TypeID type, Entity targetEnt):
    ISerializeable{},
    typeID{type},
    target{targetEnt} {

}

void Event::LoadFromJSON(const nlohmann::json& json) {

}

nlohmann::json Event::SaveToJSON() const {
    return nlohmann::json{};
}

Event::TypeID Event::GetTypeID() const {
    return typeID;
}

Entity Event::GetTarget() const {
    return target;
}

EventSystem::EventSystem():
    ISystem{} {

}

void EventSystem::Update() {
    while(!queue.empty()) {
        Event nextEvent{queue.front()};
        queue.pop();
        auto& subscriberList{subscribers[(int)nextEvent.GetTypeID()]};
        for(const auto& subscriber : subscriberList) {
            subscriber->HandleEvent(nextEvent);
        }
    }
}

ISystem::SystemID EventSystem::GetSystemID() const {
    return ISystem::SystemID::EventSystem;
}

void EventSystem::Enqueue(const Event& event) {
    queue.push(event);
}

void EventSystem::Subscribe(IEventSubscriber* subscriber, Event::TypeID eventType) {
    auto& subscriberList{subscribers[(int)eventType]};
    subscriberList.push_back(subscriber);
}

void EventSystem::Unsubscribe(IEventSubscriber* subscriber, Event::TypeID eventType) {
    auto& subscriberList{subscribers[(int)eventType]};
    for(auto iter = subscriberList.begin(); iter != subscriberList.end(); ++iter) {
        if((*iter) == subscriber) {
            subscriberList.erase(iter);
            break;
        }
    }
}