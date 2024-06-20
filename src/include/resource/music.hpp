#pragma once

#include <SFML/Audio/Music.hpp>
#include "../resource/resource.hpp"

class Music: public Resource {
public:
    Music(const ResourceID& resourceID, std::string_view resourcePath);

    struct PlaybackParam {
        bool                loop;
        float               volume;
    };

    enum class MusicStatus {
        Playing,
        Paused,
        Stopped
    };
    MusicStatus             GetStatus() const;

    void                    SetLoop(bool loop);
    float                   GetVolume() const;
    void                    SetVolume(float volume);
    void                    Play();
    void                    Pause();
    void                    Stop();
private:
    sf::Music               music;
};