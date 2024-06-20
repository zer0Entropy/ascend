#pragma once

#include "../core/event.hpp"
#include "../component/text.hpp"
#include "../resource/token.hpp"
#include "../resource/music.hpp"

struct Layer {
    enum class TypeID {
        Null = 0,
        Music,
        Decoration,
        MenuButton,
        MenuLabel
    };

    int                                 index;
    std::string                         name;
    TypeID                              typeID;

    std::vector<Entity>                 entities;
    std::vector<ResourceToken>          resources;

    std::vector<sf::Rect<unsigned int>> boundingBoxes;
    std::vector<ResourceID>             spriteAttachments;
    std::vector<ResourceID>             textAttachments;
    std::vector<std::string>            textContents;
    std::vector<Text::FontParameters>   fontParameters;
    std::vector<Alignment>              labelAlignments;
    std::vector<
        std::pair<  Event::TypeID,
                    ResourceID>>        textureSwitches;
    std::vector<
        std::pair<  Event::TypeID,
                    ResourceID>>        soundEffects;
    ResourceID                          backgroundMusicID;
    Music::PlaybackParam                musicPlaybackParam;
};