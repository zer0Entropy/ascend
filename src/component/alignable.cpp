#include "../include/component/alignable.hpp"

Alignable::Alignable(Entity cEntity, Alignment align, const BoundingBox& alignBounds):
    Component{Component::TypeID::Alignable, cEntity},
    alignment{align},
    targetBoundingBox{alignBounds} {

}

void Alignable::LoadFromJSON(const nlohmann::json& json) {
    const auto& findAlignment{json.find("alignment")};
    if(findAlignment != json.end()) {
        std::string alignmentName{findAlignment.value().template get<std::string>()};
        if(alignmentName.compare(AlignmentNames.at((int)Alignment::Left)) == 0) {
            alignment = Alignment::Left;
        }
        else if(alignmentName.compare(AlignmentNames.at((int)Alignment::Center)) == 0) {
            alignment = Alignment::Center;
        }
        else if(alignmentName.compare(AlignmentNames.at((int)Alignment::Right)) == 0) {
            alignment = Alignment::Right;
        }
    }
}

nlohmann::json Alignable::SaveToJSON() const {
    std::string alignmentName{AlignmentNames.at((int)alignment)};
    const nlohmann::json output{
        {"alignment", alignmentName}
    };
    return output;
}

Alignment Alignable::GetAlignment() const {
    return alignment;
}

void Alignable::SetAlignment(Alignment align) {
    alignment = align;
}

const BoundingBox& Alignable::GetTargetBoundingBox() const {
    return targetBoundingBox;
}

void AlignableMgr::Add(Entity owner, Alignment align, const BoundingBox& bounds) {
    alignLabelMap.insert(std::make_pair(owner, std::make_unique<Alignable>(owner, align, bounds)));
}

void AlignableMgr::Remove(Entity owner) {
    const auto& iter{alignLabelMap.find(owner)};
    if(iter != alignLabelMap.end()) {
        alignLabelMap.erase(iter);
    }
}

Alignable* AlignableMgr::Get(Entity owner) {
    const auto& iter{alignLabelMap.find(owner)};
    if(iter != alignLabelMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<Alignable*> AlignableMgr::GetList() const {
    std::vector<Alignable*>        alignLabelList;
    for(const auto& alignLabel : alignLabelMap) {
        alignLabelList.push_back(alignLabel.second.get());
    }
    return alignLabelList;
}