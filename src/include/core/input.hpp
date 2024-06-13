#pragma once

#include <vector>
#include <SFML/Window.hpp>
#include "../interface/listener.hpp"
#include "../interface/system.hpp"

class InputSystem: public ISystem {
public:
    InputSystem(sf::Window& win);

    void                                Update();
    ISystem::SystemID                   GetSystemID() const;

    void                                Subscribe(IInputListener* listener);
    void                                Unsubscribe(IInputListener* listener);
private:
    sf::Window&                         window;
    std::vector<IInputListener*>        listeners;
};