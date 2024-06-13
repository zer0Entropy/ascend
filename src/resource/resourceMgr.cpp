#include "../include/resource/resourceMgr.hpp"

Font* ResourceMgr::LoadFont(const ResourceID& resourceID, std::string_view resourcePath) {
    auto result{
        fontMap.insert(std::make_pair(resourceID, std::make_unique<Font>(resourceID, resourcePath)))
    };
    if(result.second) {
        return GetFont(resourceID);
    }
    return nullptr;
}

void ResourceMgr::UnloadFont(const ResourceID& resourceID) {
    const auto& iter{fontMap.find(resourceID)};
    if(iter != fontMap.end()) {
        fontMap.erase(iter);
    }
}

Font* ResourceMgr::GetFont(const ResourceID& resourceID) {
    const auto& iter{fontMap.find(resourceID)};
    if(iter != fontMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

TextFile* ResourceMgr::LoadTextFile(const ResourceID& resourceID, std::string_view resourcePath) {
    auto result{
        textFileMap.insert(std::make_pair(resourceID, std::make_unique<TextFile>(resourceID, resourcePath)))
    };
    if(result.second) {
        return GetTextFile(resourceID);
    }
    return nullptr;
}

void ResourceMgr::UnloadTextFile(const ResourceID& resourceID) {
    auto iter{textFileMap.find(resourceID)};
    if(iter != textFileMap.end()) {
        iter->second.reset(nullptr);
        textFileMap.erase(iter);
    }
}

TextFile* ResourceMgr::GetTextFile(const ResourceID& resourceID) {
    auto iter{textFileMap.find(resourceID)};
    if(iter != textFileMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

Texture* ResourceMgr::LoadTexture(const ResourceID& resourceID, std::string_view resourcePath) {
    auto result{
        textureMap.insert(std::make_pair(resourceID, std::make_unique<Texture>(resourceID, resourcePath)))
    };
    if(result.second) {
        return GetTexture(resourceID);
    }
    return nullptr;
}

void ResourceMgr::UnloadTexture(const ResourceID& resourceID) {
    auto iter{textureMap.find(resourceID)};
    if(iter != textureMap.end()) {
        iter->second.reset(nullptr);
        textureMap.erase(iter);
    }
}

Texture* ResourceMgr::GetTexture(const ResourceID& resourceID) {
    auto iter{textureMap.find(resourceID)};
    if(iter != textureMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}