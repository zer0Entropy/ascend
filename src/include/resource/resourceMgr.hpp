#pragma once

#include <memory>
#include <unordered_map>
#include "font.hpp"
#include "textFile.hpp"
#include "texture.hpp"

class ResourceMgr {
public:
    Font*               LoadFont(const ResourceID& resourceID, std::string_view resourcePath);
    void                UnloadFont(const ResourceID& resourceID);
    Font*               GetFont(const ResourceID& resourceID);

    TextFile*           LoadTextFile(const ResourceID& resourceID, std::string_view resourcePath);
    void                UnloadTextFile(const ResourceID& resourceID);
    TextFile*           GetTextFile(const ResourceID& resourceID);

    Texture*            LoadTexture(const ResourceID& resourceID, std::string_view resourcePath);
    void                UnloadTexture(const ResourceID& resourceID);
    Texture*            GetTexture(const ResourceID& resourceID);

private:
    std::unordered_map<ResourceID, std::unique_ptr<Font>>       fontMap;
    std::unordered_map<ResourceID, std::unique_ptr<TextFile>>   textFileMap;
    std::unordered_map<ResourceID, std::unique_ptr<Texture>>    textureMap;
};