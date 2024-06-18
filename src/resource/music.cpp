#include "../include/resource/music.hpp"

Music::Music(const ResourceID& resourceID, std::string_view resourcePath):
    Resource{resourceID, Resource::TypeID::Music, resourcePath},
    status{MusicStatus::Stopped} {
    music.openFromFile(this->GetPath());
}

Music::MusicStatus Music::GetStatus() const {
    return status;
}

void Music::Play() {
    music.play();
    status = MusicStatus::Playing;
}

void Music::Pause() {
    music.pause();
    status = MusicStatus::Paused;
}

void Music::Stop() {
    music.stop();
    status = MusicStatus::Stopped;
}