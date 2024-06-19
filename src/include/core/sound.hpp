#pragma once

#include "event.hpp"
#include "../interface/subscriber.hpp"
#include "../interface/system.hpp"
#include "../resource/sound.hpp"

struct SoundEffectToken {
    Event::TypeID       triggerEvent;
    ResourceID          soundID;
};

class ResourceMgr;

class SoundSystem: public ISystem, public IEventSubscriber {
public:
    SoundSystem(ResourceMgr& resMgr, EventSystem& evSys);

    void                Update() override;

    ISystem::SystemID   GetSystemID() const override;

    void                HandleEvent(const Event& event) override;

    Sound*              GetCurrentSound() const;
    void                SetSound(const ResourceID& soundID);
    void                SetSound(Sound* sound);
    void                Reset();

    void                SetVolume(float vol);
    float               GetVolume() const;

    void                Play();
    void                Pause();
    void                Stop();

    void                AddSoundEffect(const SoundEffectToken& soundEffect);
    void                RemoveSoundEffect(Event::TypeID triggerEvent);
    Sound*              GetTriggeredSound(Event::TypeID triggerEvent) const;

private:
    ResourceMgr&        resourceMgr;
    EventSystem&        eventSystem;
    Sound*              currentSound;
    float               volume;
    
    std::unordered_map<Event::TypeID, Sound*>       soundEffectMap;
};