#include "../include/core/sound.hpp"
#include "../include/resource/resourceMgr.hpp"

SoundSystem::SoundSystem(ResourceMgr& resMgr, EventSystem& evSys):
    ISystem{},
    resourceMgr{resMgr},
    eventSystem{evSys},
    currentSound{nullptr},
    volume{100.0f} {

}

void SoundSystem::Update() {

}

ISystem::SystemID SoundSystem::GetSystemID() const {
    return ISystem::SystemID::SoundSystem;
}

void SoundSystem::HandleEvent(const Event& event) {
    bool                    hasMatchingTrigger{false};
    for(const auto& soundEffect : soundEffectMap) {
        if(event.GetTypeID() == soundEffect.first) {
            hasMatchingTrigger = true;
            break;
        }
    }
    if(!hasMatchingTrigger) {
        return;
    }
    Sound*                  triggeredSound{GetTriggeredSound(event.GetTypeID())};
    SetSound(triggeredSound);
    Play();
    currentSound = nullptr;
}

Sound* SoundSystem::GetCurrentSound() const {
    return currentSound;
}

void SoundSystem::SetSound(const ResourceID& soundID) {
    Sound* sound{resourceMgr.GetSound(soundID)};
    if(sound) {
        Stop();
        currentSound = sound;
        currentSound->SetVolume(volume);
    }
}

void SoundSystem::SetSound(Sound* sound) {
    Stop();
    currentSound = sound;
    if(currentSound) {
        currentSound->SetVolume(volume);
    }
}

void SoundSystem::Reset() {
    if(currentSound) {
        currentSound->Stop();
        currentSound = nullptr;
    }
}

void SoundSystem::SetVolume(float vol) {
    volume = vol;
    if(currentSound) {
        currentSound->SetVolume(volume);
    }
}

float SoundSystem::GetVolume() const {
    return volume;
}

void SoundSystem::Play() {
    if(currentSound) {
        currentSound->Play();
    }
}

void SoundSystem::Pause() {
    if(currentSound) {
        currentSound->Pause();
    }
}

void SoundSystem::Stop() {
    if(currentSound) {
        currentSound->Stop();
    }
}

void SoundSystem::AddSoundEffect(const SoundEffectToken& soundEffect) {
    Sound* sound{resourceMgr.GetSound(soundEffect.soundID)};
    if(sound) {
        soundEffectMap.insert(std::make_pair(soundEffect.triggerEvent, sound));
    }
}

void SoundSystem::RemoveSoundEffect(Event::TypeID triggerEvent) {
    const auto& iter{soundEffectMap.find(triggerEvent)};
    if(iter != soundEffectMap.end()) {
        soundEffectMap.erase(iter);
    }
}

Sound* SoundSystem::GetTriggeredSound(Event::TypeID triggerEvent) const {
    const auto& iter{soundEffectMap.find(triggerEvent)};
    if(iter != soundEffectMap.end()) {
        return iter->second;
    }
    return nullptr;
}