#include "../include/component/scale.hpp"
#include "../include/resource/texture.hpp"

ScaleRenderable::ScaleRenderable(Entity cEntity, const BoundingBox& boundsCmp, Sprite& spriteCmp):
    Component{Component::TypeID::ScaleRenderable, cEntity},
    boundingBox{boundsCmp},
    sprite{spriteCmp} {
    
    const auto& attachments{sprite.GetAttachments()};
    if(!attachments.empty()) {
        Resource* attachedResource{*attachments.begin()};
        if(attachedResource->GetTypeID() == Resource::TypeID::Texture) {
            Texture* texture{dynamic_cast<Texture*>(attachedResource)};
            const auto& textureSize{texture->GetTexture().getSize()};
            scalingFactor.x = (float)boundingBox.GetWidth() / (float)textureSize.x;
            scalingFactor.y = (float)boundingBox.GetHeight() / (float)textureSize.y;
        }
    }
}

void ScaleRenderable::LoadFromJSON(const nlohmann::json& json) {

}
    
nlohmann::json ScaleRenderable::SaveToJSON() const {
    return nlohmann::json{};
}

const sf::Vector2f& ScaleRenderable::GetScalingFactor() const {
    return scalingFactor;
}

void ScaleRenderableMgr::Add(Entity owner, const BoundingBox& boundsCmp, Sprite& spriteCmp) {
    scaleMap.insert(std::make_pair(owner, std::make_unique<ScaleRenderable>(owner, boundsCmp, spriteCmp)));
}

void ScaleRenderableMgr::Remove(Entity owner) {
    const auto& iter{scaleMap.find(owner)};
    if(iter != scaleMap.end()) {
        scaleMap.erase(iter);
    }
}

ScaleRenderable* ScaleRenderableMgr::Get(Entity owner) {
    const auto& iter{scaleMap.find(owner)};
    if(iter != scaleMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<ScaleRenderable*> ScaleRenderableMgr::GetList() const {
    std::vector<ScaleRenderable*>       scaleList;
    for(const auto& iter : scaleMap) {
        scaleList.push_back(iter.second.get());
    }
    return                              scaleList;
}