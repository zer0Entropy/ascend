#pragma once

#include "../interface/system.hpp"
#include "../resource/sound.hpp"

class ResourceMgr;

class SoundSystem: public ISystem {
public:
    SoundSystem(ResourceMgr& resMgr);

    void                Update() override;

    ISystem::SystemID   GetSystemID() const override;

    Sound*              GetCurrentSound() const;
    void                SetSound(const ResourceID& soundID);
    void                Reset();

    void                SetVolume(float vol);
    float               GetVolume() const;

    void                Play();
    void                Pause();
    void                Stop();

private:
    ResourceMgr&        resourceMgr;
    Sound*              currentSound;
    float               volume;
};