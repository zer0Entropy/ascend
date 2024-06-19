#include "../include/core/sound.hpp"
#include "../include/resource/resourceMgr.hpp"

SoundSystem::SoundSystem(ResourceMgr& resMgr):
    ISystem{},
    resourceMgr{resMgr},
    currentSound{nullptr},
    volume{100.0f} {

}

void SoundSystem::Update() {

}

ISystem::SystemID SoundSystem::GetSystemID() const {
    return ISystem::SystemID::SoundSystem;
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