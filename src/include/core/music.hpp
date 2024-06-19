#pragma once

#include "../interface/system.hpp"
#include "../resource/resource.hpp"

class Music;
class ResourceMgr;

class MusicSystem: public ISystem {
public:
    MusicSystem(ResourceMgr& resMgr);

    void            Update() override;

    SystemID        GetSystemID() const override;

    Music*          GetCurrentMusic() const;
    void            SetMusic(const ResourceID& musicID);
    void            Reset();

    void            SetLoop(bool loop);
    void            SetVolume(float vol);
    float           GetVolume() const;

    void            Play();
    void            Pause();
    void            Stop();

private:
    ResourceMgr&    resourceMgr;
    Music*          currentMusic;
    float           volume;
};