#pragma once

#include <SFML/Audio/Music.hpp>
#include "../resource/resource.hpp"

class Music: public Resource {
public:
    Music(const ResourceID& resourceID, std::string_view resourcePath);

    enum class MusicStatus {
        Playing,
        Paused,
        Stopped
    };
    MusicStatus         GetStatus() const;

    void                Play();
    void                Pause();
    void                Stop();
private:
    sf::Music           music;
    MusicStatus         status;
};