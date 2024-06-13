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
    for(auto attachmentJSON : findAttachments.value()) {
        const auto& findResource{attachmentJSON.find("resource")};
        if(findResource != attachmentJSON.end()) {
            const auto& resourceJSON{findResource.value()};
            const auto& findID{resourceJSON.find("id")};
            const auto& findTypeID{resourceJSON.find("typeID")};
            const auto& findPath{resourceJSON.find("path")};
            ResourceID id{""};
            Resource::TypeID type;
            std::string path{""};
            if(findID != resourceJSON.end()) {
                findID.value().get_to(id);
            }
            if(findTypeID != resourceJSON.end()) {
                type = (Resource::TypeID)findTypeID.value();
            }
            if(findPath != resourceJSON.end()) {
                findPath.value().get_to(path);
            }
            if(     id.length() > 0
                &&  path.length() > 0) {
                Resource* resource{nullptr};
                if(type == Resource::TypeID::Texture) {
                    resource = resourceMgr.GetTexture(id);
                    if(!resource) {
                        resource = resourceMgr.LoadTexture(id, path);
                    }
                }
                if(resource) {
                    this->Attach(resource);
                }
            }
        }
    }
}

nlohmann::json Sprite::SaveToJSON() const {
    nlohmann::json attachmentJSON;
    const auto& attachmentList{this->GetAttachments()};
    if(attachmentList.size() > 0) {
        Resource* resource{*attachmentList.begin()};
        attachmentJSON = resource->SaveToJSON();
    }
    const nlohmann::json output{
            {"attachments", {
                attachmentJSON
            } }
    };
    return output;
}

void Sprite::Attach(Resource* resource) {
    const auto& attached{this->GetAttachments()};
    if(     attached.size() == 0
        &&  resource->GetTypeID() == Resource::TypeID::Texture) {
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