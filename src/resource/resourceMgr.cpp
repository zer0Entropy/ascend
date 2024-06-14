#include <iomanip>
#include "../include/resource/resourceMgr.hpp"

Font* ResourceMgr::LoadFont(const ResourceID& resourceID, std::string_view resourcePath) {
    auto result{
        fontMap.insert(std::make_pair(resourceID, std::make_unique<Font>(resourceID, resourcePath)))
    };
    if(result.second) {
        std::string msg{"Font \"" + resourceID + "\" successfully loaded from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg);
        return GetFont(resourceID);
    }
    return nullptr;
}

void ResourceMgr::UnloadFont(const ResourceID& resourceID) {
    const auto& iter{fontMap.find(resourceID)};
    if(iter != fontMap.end()) {
        iter->second.reset(nullptr);
        fontMap.erase(iter);
        std::string msg{"Font \"" + resourceID + "\" was successfully unloaded."};
        this->PublishMsg(msg);
    }
}

Font* ResourceMgr::GetFont(const ResourceID& resourceID) {
    const auto& iter{fontMap.find(resourceID)};
    if(iter != fontMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

TextFile* ResourceMgr::LoadTextFile(const ResourceID& resourceID, std::string_view resourcePath, bool overwrite) {
    auto result{
        textFileMap.insert(std::make_pair(resourceID, std::make_unique<TextFile>(resourceID, resourcePath, overwrite)))
    };
    if(result.second) {
        std::string msg{"TextFile \"" + resourceID + "\" successfully loaded from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg);
        return GetTextFile(resourceID);
    }
    return nullptr;
}

void ResourceMgr::UnloadTextFile(const ResourceID& resourceID) {
    auto iter{textFileMap.find(resourceID)};
    if(iter != textFileMap.end()) {
        iter->second.reset(nullptr);
        textFileMap.erase(iter);
        std::string msg{"TextFile \"" + resourceID + "\" was successfully unloaded."};
        this->PublishMsg(msg);
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
        std::string msg{"Texture \"" + resourceID + "\" successfully loaded from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg);
        return GetTexture(resourceID);
    }
    return nullptr;
}

void ResourceMgr::UnloadTexture(const ResourceID& resourceID) {
    auto iter{textureMap.find(resourceID)};
    if(iter != textureMap.end()) {
        iter->second.reset(nullptr);
        textureMap.erase(iter);
        std::string msg{"Texture \"" + resourceID + "\" was successfully unloaded."};
        this->PublishMsg(msg);
    }
}

Texture* ResourceMgr::GetTexture(const ResourceID& resourceID) {
    auto iter{textureMap.find(resourceID)};
    if(iter != textureMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}