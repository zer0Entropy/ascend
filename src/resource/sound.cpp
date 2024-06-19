#include "../include/resource/sound.hpp"

Sound::Sound(const ResourceID& resourceID, std::string_view resourcePath):
    Resource{resourceID, Resource::TypeID::Sound, resourcePath} {
    buffer.loadFromFile(this->GetPath());
    sound.setBuffer(buffer);
}

Sound::SoundStatus Sound::GetStatus() const {
    sf::SoundSource::Status internalState{sound.getStatus()};
    if(internalState == sf::SoundSource::Status::Playing) {
        return SoundStatus::Playing;
    }
    else if(internalState == sf::SoundSource::Status::Paused) {
        return SoundStatus::Paused;
    }
    return SoundStatus::Stopped;
}

float Sound::GetVolume() const {
    return sound.getVolume();
}

void Sound::SetVolume(float volume) {
    sound.setVolume(volume);
}

void Sound::Play() {
    sound.play();
}

void Sound::Pause() {
    sound.pause();
}

void Sound::Stop() {
    sound.stop();
}