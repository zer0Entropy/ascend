#pragma once

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void        Update() = 0;

    enum class SystemID {
        LogSystem,
        InputSystem,
        RenderSystem,
        EventSystem,
        MusicSystem,
        SoundSystem
    };
    virtual SystemID    GetSystemID() const = 0;
};

constexpr int NumSystems{6};