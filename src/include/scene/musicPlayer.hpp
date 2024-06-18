#pragma once

#include "../interface/attachment.hpp"
#include "../resource/music.hpp"

class MusicPlayer: public ICanHaveAttachments {
public:
    MusicPlayer();
    MusicPlayer(const MusicPlayer& copy) = default;
    MusicPlayer(MusicPlayer&& move) = default;
    ~MusicPlayer() = default;
    MusicPlayer& operator=(const MusicPlayer& copy) = default;
    MusicPlayer& operator=(MusicPlayer&& move) = default;

    void            Attach(Resource* resource) override;
    void            Detatch(const ResourceID& resourceID) override;

    void            Play();
    void            Pause();
    void            Stop();
    Music*          GetMusic() const;
private:
    Music*          music;
};