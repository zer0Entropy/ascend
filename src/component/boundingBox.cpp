#include "../include/component/boundingBox.hpp"

BoundingBox::BoundingBox(Entity cOwner):
    Component{Component::TypeID::BoundingBox, cOwner},
    boundsRect{0, 0, 0, 0} {

}

unsigned int BoundingBox::GetTop() const {
    return boundsRect.top;
}

unsigned int BoundingBox::GetLeft() const {
    return boundsRect.left;
}

unsigned int BoundingBox::GetWidth() const {
    return boundsRect.width;
}

unsigned int BoundingBox::GetHeight() const {
    return boundsRect.height;
}

void BoundingBox::LoadFromJSON(const nlohmann::json& json) {
    const auto& findRect{json.find("boundsRect")};
    if(findRect != json.end()) {
        const auto& rectJSON{findRect.value()};
        const auto& findLeft{rectJSON.find("left")};
        const auto& findTop{rectJSON.find("top")};
        const auto& findWidth{rectJSON.find("width")};
        const auto& findHeight{rectJSON.find("height")};
        if(findLeft != rectJSON.end()) {
            boundsRect.left = findLeft.value();
        }
        if(findTop != rectJSON.end()) {
            boundsRect.top = findTop.value();
        }
        if(findWidth != rectJSON.end()) {
            boundsRect.width = findWidth.value();
        }
        if(findHeight != rectJSON.end()) {
            boundsRect.height = findHeight.value();
        }
    }
}
    
nlohmann::json BoundingBox::SaveToJSON() const {
    const nlohmann::json output{
            {"boundsRect", {
                {"left", boundsRect.left},
                {"top", boundsRect.top},
                {"width", boundsRect.width},
                {"height", boundsRect.height}
            } }
    };
    return output;
}

const sf::Rect<unsigned int>& BoundingBox::GetRect() const {
    return boundsRect;
}

void BoundingBox::SetRect(const sf::Rect<unsigned int>& rect) {
    boundsRect = rect;
}

void BoundingBoxMgr::Add(Entity owner, const sf::Rect<unsigned int>& boundsRect) {
    boundsMap.insert(std::make_pair(owner, std::make_unique<BoundingBox>(owner)));
    Get(owner)->SetRect(boundsRect);
}

void BoundingBoxMgr::Remove(Entity owner) {
    auto iter{boundsMap.find(owner)};
    if(iter != boundsMap.end()) {
        boundsMap.erase(iter);
    }
}

BoundingBox* BoundingBoxMgr::Get(Entity owner) {
    auto iter{boundsMap.find(owner)};
    if(iter != boundsMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<BoundingBox*> BoundingBoxMgr::GetList() const {
    std::vector<BoundingBox*> boundsList;
    for(const auto& bounds : boundsMap) {
        boundsList.push_back(bounds.second.get());
    }
    return boundsList;
}