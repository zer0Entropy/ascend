#include <SFML/Window/Event.hpp>
#include "../include/core/inputHandler.hpp"

InputHandler::InputHandler():
    IInputListener{},
    IThrowSignal{} {

}

bool InputHandler::ReceiveInput(const sf::Event& event) {
    bool handled{false};
    if(event.type == sf::Event::Closed) {
        ThrowSignal(SignalID::WindowClosed);
        handled = true;
    }
    return handled;
}