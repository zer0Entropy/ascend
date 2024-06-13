#pragma once

enum class SignalID {
    FatalError,
    WindowClosed
};

class Application;

class IThrowSignal {
public:
    ~IThrowSignal() = default;
    virtual void        ThrowSignal(SignalID signal);
};