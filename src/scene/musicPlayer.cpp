#include "../include/scene/musicPlayer.hpp"

MusicPlayer::MusicPlayer():
    ICanHaveAttachments{},
    music{nullptr} {

}

void MusicPlayer::Attach(Resource* resource) {
    if(     resource->GetTypeID() == Resource::TypeID::Music
        &&  this->GetAttachments().empty()) {
        ICanHaveAttachments::Attach(resource);
        music = dynamic_cast<Music*>(resource);
    }
}

void MusicPlayer::Detatch(const ResourceID& resourceID) {
    ICanHaveAttachments::Detatch(resourceID);
    if(this->GetAttachments().empty()) {
        music = nullptr;
    }
}

void MusicPlayer::Play() {
    if(music) {
        music->Play();
    }
}

void MusicPlayer::Pause() {
    if(music) {
        music->Pause();
    }
}

void MusicPlayer::Stop() {
    if(music) {
        music->Stop();
    }
}

Music* MusicPlayer::GetMusic() const {
    return music;
}