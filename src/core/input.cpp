#include "../include/core/input.hpp"

InputSystem::InputSystem(sf::Window& win):
    ISystem{},
    window{win} {

}

void InputSystem::Update() {
    sf::Event event;
    while(window.pollEvent(event)) {
        if(     event.type == sf::Event::Closed
            ||  event.type == sf::Event::MouseMoved
            ||  event.type == sf::Event::MouseButtonPressed
            ||  event.type == sf::Event::MouseButtonReleased) {
            for(auto listener : listeners) {
                bool done{listener->ReceiveInput(event)};
                if(done) {
                    break;
                }
            }
        }
    }
}

ISystem::SystemID InputSystem::GetSystemID() const {
    return ISystem::SystemID::InputSystem;
}

void InputSystem::Subscribe(IInputListener* listener) {
    listeners.push_back(listener);
}

void InputSystem::Unsubscribe(IInputListener* listener) {
    for(auto iter = listeners.begin(); iter != listeners.end(); ++iter) {
        if((*iter) == listener) {
            listeners.erase(iter);
            break;
        }
    }
}