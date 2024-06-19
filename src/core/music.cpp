#include "../include/core/music.hpp"
#include "../include/resource/resourceMgr.hpp"

MusicSystem::MusicSystem(ResourceMgr& resMgr):
    ISystem{},
    resourceMgr{resMgr},
    currentMusic{nullptr},
    volume{100.0f} {

}

void MusicSystem::Update() {
    
}

ISystem::SystemID MusicSystem::GetSystemID() const {
    return ISystem::SystemID::MusicSystem;
}

Music* MusicSystem::GetCurrentMusic() const {
    return currentMusic;
}

void MusicSystem::SetMusic(const ResourceID& musicID) {
    Music* music{resourceMgr.GetMusic(musicID)};
    if(music) {
        Stop();
        currentMusic = music;
        currentMusic->SetVolume(volume);
    }
}

void MusicSystem::Reset() {
    if(currentMusic) {
        currentMusic->Stop();
        currentMusic = nullptr;
    }
}

void MusicSystem::SetLoop(bool loop) {
    if(currentMusic) {
        currentMusic->SetLoop(loop);
    }
}

void MusicSystem::SetVolume(float vol) {
    volume = vol;
    if(currentMusic) {
        currentMusic->SetVolume(volume);
    }
}

float MusicSystem::GetVolume() const {
    return volume;
}

void MusicSystem::Play() {
    if(currentMusic) {
        currentMusic->Play();
    }
}

void MusicSystem::Pause() { 
    if(currentMusic) {
        currentMusic->Pause();
    }
}

void MusicSystem::Stop() {
    if(currentMusic) {
        currentMusic->Stop();
    }
}
