#include "../include/resource/resource.hpp"

Resource::Resource(const ResourceID& resID, TypeID resType, std::string_view resPath):
    ISerializeable{},
    id{resID}, typeID{resType}, path{resPath} {

}

const ResourceID& Resource::GetID() const {
    return id;
}

Resource::TypeID Resource::GetTypeID() const {
    return typeID;
}

const std::string& Resource::GetPath() const {
    return path;
}


void Resource::LoadFromJSON(const nlohmann::json& json) {

}

nlohmann::json Resource::SaveToJSON() const {
    const nlohmann::json output{
        {"resource", {
            {"id", this->GetID()},
            {"typeID", (int)this->GetTypeID()},
            {"path", this->GetPath()}
        } }
    };
    return output;
}
