#include "../include/component/scale.hpp"
#include "../include/component/sprite.hpp"
#include "../include/component/text.hpp"
#include "../include/resource/texture.hpp"

ScaleRenderable::ScaleRenderable(Entity cEntity, const Renderable& renderableCmp):
    Component{Component::TypeID::ScaleRenderable, cEntity},
    renderable{renderableCmp} {
    // Acquire originalSize from the Sprite or Text pointed to by renderable->drawable
    sf::Vector2u originalSize{0, 0};
    Sprite* sprite{renderableCmp.GetSprite()};
    if(sprite) {
        originalSize.x = sprite->GetSprite().getGlobalBounds().width;
        originalSize.y = sprite->GetSprite().getGlobalBounds().height;
    }

    // targetSize is defined by our BoundingBox
    const BoundingBox& boundingBox{renderable.GetBounds()};
    const sf::Vector2u targetSize{
        boundingBox.GetRect().width,
        boundingBox.GetRect().height
    };

    if(originalSize.x > 0 && originalSize.y > 0) {
        scalingFactor.x = (float)targetSize.x / (float)originalSize.x;
        scalingFactor.y = (float)targetSize.y / (float)originalSize.y;
    }
    else {
        scalingFactor.x = 1.0f;
        scalingFactor.y = 1.0f;
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

void ScaleRenderableMgr::Add(Entity owner, const Renderable& renderableCmp) {
    scaleMap.insert(std::make_pair(owner, std::make_unique<ScaleRenderable>(owner, renderableCmp)));
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