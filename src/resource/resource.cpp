#include "../include/resource/resource.hpp"

Resource::Resource(const ResourceID& resID, TypeID resType, std::string_view resPath):
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

