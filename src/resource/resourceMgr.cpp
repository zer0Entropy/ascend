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
    else {
        std::string errorMsg{"File not found: " + std::string{resourcePath} + "."};
        this->PublishMsg(errorMsg, MsgPriorityID::Error);
        std::string msg{"Font \"" + resourceID + "\" failed to load from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg, MsgPriorityID::Error);
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
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to unload the Font \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
    }
}

Font* ResourceMgr::GetFont(const ResourceID& resourceID) {
    const auto& iter{fontMap.find(resourceID)};
    if(iter != fontMap.end()) {
        return iter->second.get();
    }
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to retrieve the Font \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
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
    else {
        std::string errorMsg{"File not found: " + std::string{resourcePath} + "."};
        this->PublishMsg(errorMsg, MsgPriorityID::Error);
        std::string msg{"TextFile \"" + resourceID + "\" failed to load from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg, MsgPriorityID::Error);
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
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to unload the TextFile \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
    }
}

TextFile* ResourceMgr::GetTextFile(const ResourceID& resourceID) {
    auto iter{textFileMap.find(resourceID)};
    if(iter != textFileMap.end()) {
        return iter->second.get();
    }
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to retrieve the TextFile \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
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
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to unload the Texture \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
    }
}

Texture* ResourceMgr::GetTexture(const ResourceID& resourceID) {
    auto iter{textureMap.find(resourceID)};
    if(iter != textureMap.end()) {
        return iter->second.get();
    }
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to retrieve the Texture \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
    }
    return nullptr;
}