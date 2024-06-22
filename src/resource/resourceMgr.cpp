#include <iomanip>
#include "../include/resource/resourceMgr.hpp"
#include "../include/resource/compositeTexture.hpp"
#include "../include/resource/repeatingTexture.hpp"

Font* ResourceMgr::LoadFont(const ResourceID& resourceID, std::string_view resourcePath) {
    bool alreadyExists{false};
    const auto& iter{fontMap.find(resourceID)};
    if(iter != fontMap.end()) {
        alreadyExists = true;
    }
    auto result{
        fontMap.insert(std::make_pair(resourceID, std::make_unique<Font>(resourceID, resourcePath)))
    };
    if(result.second) {
        std::string msg{"Font \"" + resourceID + "\" successfully loaded from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg);
        return GetFont(resourceID);
    }
    else {
        std::string errorMsg{""};
        if(alreadyExists) {
            errorMsg = "Font with ResourceID=\"" + resourceID + "\" already exists!";
        }
        else {
            errorMsg = "File not found: " + std::string{resourcePath} + ".";
        };
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

JSONDocument* ResourceMgr::LoadJSONDocument(const ResourceID& resourceID, std::string_view resourcePath) {
    bool alreadyExists{false};
    const auto& iter{jsonDocMap.find(resourceID)};
    if(iter != jsonDocMap.end()) {
        alreadyExists = true;
    }
    auto result{
        jsonDocMap.insert(std::make_pair(resourceID, std::make_unique<JSONDocument>(resourceID, resourcePath)))
    };
    if(result.second) {
        std::string msg{"JSONDocument \"" + resourceID + "\" successfully loaded from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg);
        return GetJSONDocument(resourceID);
    }
    else {
        std::string errorMsg{""};
        if(alreadyExists) {
            errorMsg = "JSONDocument with ResourceID=\"" + resourceID + "\"already exists!";
        }
        else {
            errorMsg = "File not found: " + std::string{resourcePath} + ".";
        }
        this->PublishMsg(errorMsg, MsgPriorityID::Error);
        std::string msg{"JSONDocument \"" + resourceID + "\" failed to load from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg, MsgPriorityID::Error);
    }
    return nullptr;
}

void ResourceMgr::UnloadJSONDocument(const ResourceID& resourceID) {
    const auto& iter{jsonDocMap.find(resourceID)};
    if(iter != jsonDocMap.end()) {
        iter->second.reset(nullptr);
        jsonDocMap.erase(iter);
        std::string msg{"JSONDoucment \"" + resourceID + "\" was successfully unloaded."};
        this->PublishMsg(msg);
    }
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to unload the JSONDocument \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
    }
}

JSONDocument* ResourceMgr::GetJSONDocument(const ResourceID& resourceID) {
    const auto& iter{jsonDocMap.find(resourceID)};
    if(iter != jsonDocMap.end()) {
        return iter->second.get();
    }
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to retrieve the JSONDocument \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
    }
    return nullptr;
}

TextFile* ResourceMgr::LoadTextFile(const ResourceID& resourceID, std::string_view resourcePath, bool overwrite) {
    bool alreadyExists{false};
    const auto& iter{textFileMap.find(resourceID)};
    if(iter != textFileMap.end()) {
        alreadyExists = true;
    }
    auto result{
        textFileMap.insert(std::make_pair(resourceID, std::make_unique<TextFile>(resourceID, resourcePath, overwrite)))
    };
    if(result.second) {
        std::string msg{"TextFile \"" + resourceID + "\" successfully loaded from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg);
        return GetTextFile(resourceID);
    }
    else {
        std::string errorMsg{""};
        if(alreadyExists) {
            errorMsg = "TextFile with ResourceID=\"" + resourceID + "\" already exists!";
        }
        else {
            errorMsg = "File not found: " + std::string{resourcePath} + ".";
        }
        this->PublishMsg(errorMsg, MsgPriorityID::Error);
        std::string msg{"TextFile \"" + resourceID + "\" failed to load from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg, MsgPriorityID::Error);
    }
    return nullptr;
}

void ResourceMgr::UnloadTextFile(const ResourceID& resourceID) {
    const auto& iter{textFileMap.find(resourceID)};
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
    const auto& iter{textFileMap.find(resourceID)};
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

Texture* ResourceMgr::LoadTexture(  const ResourceID& resourceID,
                                    std::string_view resourcePath,
                                    const sf::IntRect pixelRect) {
    bool alreadyExists{false};
    const auto& iter{textureMap.find(resourceID)};
    if(iter != textureMap.end()) {
        alreadyExists = true;
    }
    auto result{
        textureMap.insert(std::make_pair(resourceID, std::make_unique<Texture>(resourceID, resourcePath, pixelRect)))
    };
    if(result.second) {
        std::string msg{"Texture \"" + resourceID + "\" successfully loaded from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg);
        return GetTexture(resourceID);
    }
    else {
        std::string errorMsg{""};
        if(alreadyExists) {
            errorMsg = "Texture with ResourceID=\"" + resourceID + "\" already exists!";
        }
        else {
            errorMsg = "File not found: " + std::string{resourcePath} + ".";
        }
        this->PublishMsg(errorMsg, MsgPriorityID::Error);
        std::string msg{"Texture \"" + resourceID + "\" failed to load from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg, MsgPriorityID::Error);
    }
    return nullptr;
}

Texture* ResourceMgr::LoadTexture(  const ResourceID& resourceID,
                                    std::string_view resourcePath,
                                    Orientation orientation,
                                    unsigned int numRepetitions,
                                    const sf::IntRect pixelRect) {
    bool alreadyExists{false};
    const auto& iter{textureMap.find(resourceID)};
    if(iter != textureMap.end()) {
        alreadyExists = true;
    }
    auto result{
        textureMap.insert(std::make_pair(resourceID, std::make_unique<RepeatingTexture>(
                                                        resourceID,
                                                        resourcePath,
                                                        orientation,
                                                        numRepetitions,
                                                        pixelRect)))
    };
    if(result.second) {
        std::string msg{"Texture \"" + resourceID + "\" successfully loaded from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg);
        return GetTexture(resourceID);
    }
    else {
        std::string errorMsg{""};
        if(alreadyExists) {
            errorMsg = "Texture with ResourceID=\"" + resourceID + "\" already exists!";
        }
        else {
            errorMsg = "File not found: " + std::string{resourcePath} + ".";
        }
        this->PublishMsg(errorMsg, MsgPriorityID::Error);
        std::string msg{"Texture \"" + resourceID + "\" failed to load from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg, MsgPriorityID::Error);
    }
    return nullptr;
}

Texture* ResourceMgr::LoadTexture(  const ResourceID& resourceID,
                                    std::string_view resourcePath,
                                    sf::Vector2u repeatRect,
                                    const sf::IntRect pixelRect) {
    bool alreadyExists{false};
    const auto& iter{textureMap.find(resourceID)};
    if(iter != textureMap.end()) {
        alreadyExists = true;
    }
    auto result{
        textureMap.insert(std::make_pair(resourceID, std::make_unique<RepeatingTexture>(
                                                        resourceID,
                                                        resourcePath,
                                                        repeatRect,
                                                        pixelRect)))
    };
    if(result.second) {
        std::string msg{"Texture \"" + resourceID + "\" successfully loaded from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg);
        return GetTexture(resourceID);
    }
    else {
        std::string errorMsg{""};
        if(alreadyExists) {
            errorMsg = "Texture with ResourceID=\"" + resourceID + "\" already exists!";
        }
        else {
            errorMsg = "File not found: " + std::string{resourcePath} + ".";
        }
        this->PublishMsg(errorMsg, MsgPriorityID::Error);
        std::string msg{"Texture \"" + resourceID + "\" failed to load from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg, MsgPriorityID::Error);
    }
    return nullptr;
}

Texture* ResourceMgr::LoadTexture(  const ResourceID& resourceID,
                                    const sf::Vector2u& size,
                                    const std::vector<ResourceID>& sourceTextureIDs,
                                    const std::vector<sf::Vector2u>& destinations) {
    bool alreadyExists{false};
    const auto& iter{textureMap.find(resourceID)};
    if(iter != textureMap.end()) {
        alreadyExists = true;
    }
    std::vector<Texture*> sourceTextures;
    for(const auto& srcTextureID : sourceTextureIDs) {
        Texture* texture = GetTexture(srcTextureID);
        if(texture) {
            sourceTextures.push_back(texture);
        }
    }
    std::string errorMsg{""};
    auto result{
        textureMap.insert(std::make_pair(resourceID, std::make_unique<CompositeTexture>(resourceID, size)))
    };
    if(result.second) {
        if(!sourceTextures.empty()) {
            CompositeTexture& composite{*dynamic_cast<CompositeTexture*>(GetTexture(resourceID))};
            int index = 0;
            for(const auto& destination : destinations) {
                composite.AddTexture(sourceTextures[index++]->GetTexture(), destination);
            }
            std::string msg{"Texture \"" + resourceID + "\" successfully loaded (as a composite of previously-loaded textures)."};
            this->PublishMsg(msg);
            return GetTexture(resourceID);
        }
        else {
            errorMsg = "SourceTextures not found!";
        }
    }
    else {
        if(alreadyExists) {
            errorMsg = "Texture with ResourceID=\"" + resourceID + "\" already exists!";
        }
        else {
            errorMsg = "Unknown error!";
        }
    }
    this->PublishMsg(errorMsg, MsgPriorityID::Error);
    std::string msg{"Texture \"" + resourceID + "\" failed to load from these SourceTextures:\n"};
    for(const auto& sourceTextureID : sourceTextureIDs) {
        msg.append("\t");
        msg.append(sourceTextureID);
        msg.append("\n");
    }
    this->PublishMsg(msg, MsgPriorityID::Error);
    return nullptr;
}

void ResourceMgr::UnloadTexture(const ResourceID& resourceID) {
    const auto& iter{textureMap.find(resourceID)};
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
    const auto& iter{textureMap.find(resourceID)};
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

Music* ResourceMgr::LoadMusic(const ResourceID& resourceID, std::string_view resourcePath) {
    bool alreadyExists{false};
    const auto& iter{textureMap.find(resourceID)};
    if(iter != textureMap.end()) {
        alreadyExists = true;
    }
    auto result{
        musicMap.insert(std::make_pair(resourceID, std::make_unique<Music>(resourceID, resourcePath)))
    };
    if(result.second) {
        std::string msg{"Music \"" + resourceID + "\" successfully loaded from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg);
        return GetMusic(resourceID);
    }
    else {
        std::string errorMsg{""};
        if(alreadyExists) {
            errorMsg = "Music with ResourceID=\"" + resourceID + "\" already exists!";
        }
        else {
            errorMsg = "File not found: " + std::string{resourcePath} + ".";
        }
        this->PublishMsg(errorMsg, MsgPriorityID::Error);
        std::string msg{"Music \"" + resourceID + "\" failed to load from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg, MsgPriorityID::Error);
    }
    return nullptr;
}

void ResourceMgr::UnloadMusic(const ResourceID& resourceID) {
    const auto& iter{musicMap.find(resourceID)};
    if(iter != musicMap.end()) {
        iter->second.reset(nullptr);
        musicMap.erase(iter);
        std::string msg{"Music \"" + resourceID + "\" was successfully unloaded."};
        this->PublishMsg(msg);
    }
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to unload the Music \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
    }
}

Music* ResourceMgr::GetMusic(const ResourceID& resourceID) {
    const auto& iter{musicMap.find(resourceID)};
    if(iter != musicMap.end()) {
        return iter->second.get();
    }
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to retrieve the Music \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
    }
    return nullptr;
}

Sound* ResourceMgr::LoadSound(const ResourceID& resourceID, std::string_view resourcePath) {
    bool alreadyExists{false};
    const auto& iter{textureMap.find(resourceID)};
    if(iter != textureMap.end()) {
        alreadyExists = true;
    }
    auto result{
        soundMap.insert(std::make_pair(resourceID, std::make_unique<Sound>(resourceID, resourcePath)))
    };
    if(result.second) {
        std::string msg{"Sound \"" + resourceID + "\" successfully loaded from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg);
        return GetSound(resourceID);
    }
    else {
        std::string errorMsg{""};
        if(alreadyExists) {
            errorMsg = "Music with ResourceID=\"" + resourceID + "\" already exists!";
        }
        else {
            errorMsg = "File not found: " + std::string{resourcePath} + ".";
        }
        this->PublishMsg(errorMsg, MsgPriorityID::Error);
        std::string msg{"Music \"" + resourceID + "\" failed to load from " + std::string{resourcePath} + "."};
        this->PublishMsg(msg, MsgPriorityID::Error);
    }
    return nullptr;
}

void  ResourceMgr::UnloadSound(const ResourceID& resourceID) {
    const auto& iter{soundMap.find(resourceID)};
    if(iter != soundMap.end()) {
        iter->second.reset(nullptr);
        soundMap.erase(iter);
        std::string msg{"Sound \"" + resourceID + "\" was successfully unloaded."};
        this->PublishMsg(msg);
    }
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to unload the Sound \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
    }
}

Sound* ResourceMgr::GetSound(const ResourceID& resourceID) {
    const auto& iter{soundMap.find(resourceID)};
    if(iter != soundMap.end()) {
        return iter->second.get();
    }
    else {
        std::string warnMsg{"Resource not found: " + resourceID + "."};
        this->PublishMsg(warnMsg, MsgPriorityID::Warning);
        std::string msg{"ResourceMgr received a request to retrieve the Sound \"" + resourceID + "\", but it does not exist."};
        this->PublishMsg(msg, MsgPriorityID::Warning);
    }
    return nullptr;
}