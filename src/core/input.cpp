#include "../include/core/input.hpp"

InputSystem::InputSystem(sf::Window& win):
    ISystem{},
    ILogMsgPublisher{},
    window{win} {

}

void InputSystem::Update() {
    sf::Event event;
    while(window.pollEvent(event)) {
        std::string eventMsg{""};
        if(event.type == sf::Event::Closed) {
            eventMsg = "Input event received: WindowClosed.";
        }
        else if(event.type == sf::Event::MouseButtonPressed) {
            eventMsg = "Input event received: MouseButtonPressed.";
        }
        else if(event.type == sf::Event::MouseButtonReleased) {
            eventMsg = "Input event received: MouseButtonReleased.";
        }
        if(     event.type == sf::Event::MouseMoved
            ||  eventMsg.length() > 0) {
            if(eventMsg.length() > 0) {
                PublishMsg(eventMsg);
            }
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