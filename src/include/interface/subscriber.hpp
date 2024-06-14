#pragma once

class Event;

class IEventSubscriber {
public:
    virtual ~IEventSubscriber() = default;

    virtual void            HandleEvent(const Event& event) = 0;
};