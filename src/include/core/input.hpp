#pragma once

#include <vector>
#include <SFML/Window.hpp>
#include "../interface/listener.hpp"
#include "../interface/publisher.hpp"
#include "../interface/system.hpp"

class InputSystem: public ISystem, public ILogMsgPublisher {
public:
    InputSystem(sf::Window& win);

    void                                Update() override;
    ISystem::SystemID                   GetSystemID() const override;

    void                                Subscribe(IInputListener* listener);
    void                                Unsubscribe(IInputListener* listener);
private:
    sf::Window&                         window;
    std::vector<IInputListener*>        listeners;
};