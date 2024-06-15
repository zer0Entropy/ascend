#pragma once

enum class SignalID {
    FatalError,
    WindowClosed,
    UserQuitGame
};

class Application;

class IThrowSignal {
public:
    ~IThrowSignal() = default;
    virtual void        ThrowSignal(SignalID signal);
};