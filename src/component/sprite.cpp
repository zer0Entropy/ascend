#include "../include/component/sprite.hpp"
#include "../include/core/app.hpp"

Sprite::Sprite(Entity cEntity):
    Component{Component::TypeID::Sprite, cEntity},
    ICanHaveAttachments{} {

}

sf::Sprite& Sprite::GetSprite() const {
    return const_cast<sf::Sprite&>(sprite);
}

void Sprite::LoadFromJSON(const nlohmann::json& json) {
    ResourceMgr& resourceMgr{Application::GetInstance().GetResourceMgr()};
    const auto& findAttachments{json.find("attachments")};
    if(findAttachments != json.end()) {
        const auto& attachmentsJSON{findAttachments.value()};
        const auto& findTextures{attachmentsJSON.find("textures")};
        if(findTextures != attachmentsJSON.end()) {
            for(auto textureIter : findTextures.value()) {
                ResourceID  textureID{textureIter.template get<std::string>()};
                Texture*    texture{resourceMgr.GetTexture(textureID)};
                if(texture) {
                    this->Attach(texture);
                }
            }
        }
    }
}

nlohmann::json Sprite::SaveToJSON() const {
    nlohmann::json output;
    return output;
}

void Sprite::Attach(Resource* resource) {
    const auto& attached{this->GetAttachments()};
    if(     attached.size() == 0
        && (resource->GetTypeID() == Resource::TypeID::SimpleTexture
        ||  resource->GetTypeID() == Resource::TypeID::CompositeTexture
        ||  resource->GetTypeID() == Resource::TypeID::RepeatingTexture)) {
        Texture* texture = dynamic_cast<Texture*>(resource);
        if(texture) {
            sprite.setTexture(texture->GetTexture());
            ICanHaveAttachments::Attach(texture);
        }
    }
}

void Sprite::Detatch(const ResourceID& resourceID) {
    ICanHaveAttachments::Detatch(resourceID); 
}

void SpriteMgr::Add(Entity owner) {
    spriteMap.insert(std::make_pair(owner, std::make_unique<Sprite>(owner)));
}

void SpriteMgr::Remove(Entity owner) {
    auto iter{spriteMap.find(owner)};
    if(iter != spriteMap.end()) {
        spriteMap.erase(iter);
    }
}

Sprite* SpriteMgr::Get(Entity owner) {
    auto iter{spriteMap.find(owner)};
    if(iter != spriteMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<Sprite*> SpriteMgr::GetList() const {
    std::vector<Sprite*> spriteList;
    for(const auto& sprite : spriteMap) {
        spriteList.push_back(sprite.second.get());
    }
    return spriteList;
}