#include "../include/resource/music.hpp"

Music::Music(const ResourceID& resourceID, std::string_view resourcePath):
    Resource{resourceID, Resource::TypeID::Music, resourcePath} {
    music.openFromFile(this->GetPath());
    music.setLoop(true);
}

Music::MusicStatus Music::GetStatus() const {
    sf::SoundSource::Status internalState{music.getStatus()};
    if(internalState == sf::SoundSource::Status::Playing) {
        return MusicStatus::Playing;
    }
    else if(internalState == sf::SoundSource::Status::Paused) {
        return MusicStatus::Paused;
    }
    return MusicStatus::Stopped;
}

float Music::GetVolume() const {
    return music.getVolume();
}

void Music::SetVolume(float volume) {
    music.setVolume(volume);
}

void Music::Play() {
    music.play();
}

void Music::Pause() {
    music.pause();
}

void Music::Stop() {
    music.stop();
}