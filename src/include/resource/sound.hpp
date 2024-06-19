#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "resource.hpp"

class Sound: public Resource {
public:
    Sound(const ResourceID& resourceID, std::string_view resourcePath);

    enum class SoundStatus {
        Playing,
        Paused,
        Stopped
    };
    SoundStatus             GetStatus() const;

    float                   GetVolume() const;
    void                    SetVolume(float volume);
    void                    Play();
    void                    Pause();
    void                    Stop();

private:
    sf::Sound               sound;
    sf::SoundBuffer         buffer;
};