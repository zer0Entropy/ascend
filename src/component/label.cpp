#include "../include/component/label.hpp"

Label::Label(Entity cEntity, BoundingBox& boundsCmp, Text& textCmp):
    Component{Component::TypeID::Label, cEntity},
    bounds{boundsCmp},
    text{textCmp} {

}

void Label::LoadFromJSON(const nlohmann::json& json) {

}

nlohmann::json Label::SaveToJSON() const {
    return nlohmann::json{};
}

const BoundingBox& Label::GetBounds() const {
    return const_cast<BoundingBox&>(bounds);
}

Text& Label::GetText() const {
    return const_cast<Text&>(text);
}

void LabelMgr::Add(Entity owner, BoundingBox& boundsCmp, Text& textCmp) {
    labelMap.insert(std::make_pair(owner, std::make_unique<Label>(owner, boundsCmp, textCmp)));
}

void LabelMgr::Remove(Entity owner) {
    const auto& iter{labelMap.find(owner)};
    if(iter != labelMap.end()) {
        labelMap.erase(iter);
    }
}

Label* LabelMgr::Get(Entity owner) {
    const auto& iter{labelMap.find(owner)};
    if(iter != labelMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<Label*> LabelMgr::GetList() const {
    std::vector<Label*> labelList;
    for(const auto& label : labelMap) {
        labelList.push_back(label.second.get());
    }
    return labelList;
}