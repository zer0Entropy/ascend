#include "../include/component/alignLabel.hpp"

AlignLabel::AlignLabel(Entity cEntity, Alignment align, Label& labelCmp):
    Component{Component::TypeID::AlignLabel, cEntity},
    alignment{align},
    label{labelCmp} {

}

void AlignLabel::LoadFromJSON(const nlohmann::json& json) {
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

nlohmann::json AlignLabel::SaveToJSON() const {
    std::string alignmentName{AlignmentNames.at((int)alignment)};
    const nlohmann::json output{
        {"alignment", alignmentName}
    };
    return output;
}

Alignment AlignLabel::GetAlignment() const {
    return alignment;
}

void AlignLabel::SetAlignment(Alignment align) {
    alignment = align;
}

Label& AlignLabel::GetLabel() const {
    return label;
}

void AlignLabelMgr::Add(Entity owner, Alignment align, Label& labelCmp) {
    alignLabelMap.insert(std::make_pair(owner, std::make_unique<AlignLabel>(owner, align, labelCmp)));
}

void AlignLabelMgr::Remove(Entity owner) {
    const auto& iter{alignLabelMap.find(owner)};
    if(iter != alignLabelMap.end()) {
        alignLabelMap.erase(iter);
    }
}

AlignLabel* AlignLabelMgr::Get(Entity owner) {
    const auto& iter{alignLabelMap.find(owner)};
    if(iter != alignLabelMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<AlignLabel*> AlignLabelMgr::GetList() const {
    std::vector<AlignLabel*>        alignLabelList;
    for(const auto& alignLabel : alignLabelMap) {
        alignLabelList.push_back(alignLabel.second.get());
    }
    return alignLabelList;
}