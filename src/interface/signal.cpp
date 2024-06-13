#include "../include/interface/signal.hpp"
#include "../include/core/app.hpp"

void IThrowSignal::ThrowSignal(SignalID signal) {
    Application::GetInstance().ReceiveSignal(signal);
}