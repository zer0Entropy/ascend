#include "../include/component/textureSwitcher.hpp"

TextureSwitcher::TextureSwitcher(Entity cEntity, Sprite& spriteCmp):
    Component{Component::TypeID::TextureSwitcher, cEntity},
    sprite{spriteCmp} {

}

void TextureSwitcher::LoadFromJSON(const nlohmann::json& json) {

}

nlohmann::json TextureSwitcher::SaveToJSON() const {
    return nlohmann::json{};
}

void TextureSwitcher::Attach(Resource* resource) {
    if(     resource->GetTypeID() == Resource::TypeID::SimpleTexture
        ||  resource->GetTypeID() == Resource::TypeID::CompositeTexture
        ||  resource->GetTypeID() == Resource::TypeID::RepeatingTexture) {
        ICanHaveAttachments::Attach(resource);
    }
}

void TextureSwitcher::Detatch(const ResourceID& resourceID) {
    ICanHaveAttachments::Detatch(resourceID);
}

void TextureSwitcher::HandleEvent(const Event& event) {
    Event::TypeID   eventType{event.GetTypeID()};
    const auto&     iter{triggerMap.find(eventType)};
    if(     event.GetTarget() == this->GetOwner()
        &&  iter != triggerMap.end()) {
        const auto& attachedTextures{this->GetAttachments()};
        const auto& attachmentList{sprite.GetAttachments()};
        const auto& attachedID{(*attachmentList.begin())->GetID()};
        sprite.Detatch(attachedID);

        Texture* texture{nullptr};
        for(const auto& attachedTexture : attachedTextures) {
            if(attachedTexture->GetID() == iter->second) {
                sprite.Attach(attachedTexture);
                break;
            }
        }
    }
}

ResourceID TextureSwitcher::GetTrigger(Event::TypeID triggerID) const {
    const auto& iter{triggerMap.find(triggerID)};
    if(iter != triggerMap.end()) {
        return iter->second;
    }
    return (ResourceID)"";
}

void TextureSwitcher::AddTrigger(Event::TypeID triggerID, ResourceID textureID) {
    triggerMap.insert(std::make_pair(triggerID, textureID));
}

void TextureSwitcher::RemoveTrigger(Event::TypeID triggerID) {
    const auto& iter{triggerMap.find(triggerID)};
    if(iter != triggerMap.end()) {
        triggerMap.erase(iter);
    }
}

void TextureSwitcherMgr::Add(Entity owner, Sprite& spriteCmp) {
    textureSwitcherMap.insert(std::make_pair(owner, std::make_unique<TextureSwitcher>(owner, spriteCmp)));
}

void TextureSwitcherMgr::Remove(Entity owner) {
    const auto& iter{textureSwitcherMap.find(owner)};
    if(iter != textureSwitcherMap.end()) {
        textureSwitcherMap.erase(iter);
    }
}

TextureSwitcher* TextureSwitcherMgr::Get(Entity owner) {
    const auto& iter{textureSwitcherMap.find(owner)};
    if(iter != textureSwitcherMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<TextureSwitcher*> TextureSwitcherMgr::GetList() const {
    std::vector<TextureSwitcher*> textureSwitcherList;
    for(const auto& textureSwitcher : textureSwitcherMap) {
        textureSwitcherList.push_back(textureSwitcher.second.get());
    }
    return textureSwitcherList;
}