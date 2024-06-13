#pragma once

namespace sf {
    class Event;
}

class IInputListener {
public:
    virtual ~IInputListener() = default;
    // Returns true if the input is handled by this Listener, false otherwise.
    virtual bool                ReceiveInput(const sf::Event& event) = 0;
};