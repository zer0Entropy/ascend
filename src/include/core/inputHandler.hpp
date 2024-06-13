#pragma once

#include "../interface/listener.hpp"
#include "../interface/signal.hpp"

class InputHandler: public IInputListener, public IThrowSignal {
public:
    InputHandler();

    bool        ReceiveInput(const sf::Event& event) override;
};