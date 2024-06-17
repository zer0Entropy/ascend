#pragma once

#include <string>
#include "../interface/serialize.hpp"

using ResourceID = std::string;

struct ResourceToken {
    ResourceID  id;
    std::string path;
};

class Resource {
public:
    enum class TypeID {
        Font,
        SimpleTexture,
        RepeatingTexture,
        CompositeTexture,
        TextFile
    };

    Resource() = delete;
    Resource(const ResourceID& resID, TypeID resType, std::string_view resPath);
    Resource(const Resource& copy) = default;
    Resource(Resource&& move) = default;
    virtual ~Resource() = default;
    Resource& operator=(const Resource& copy) = default;
    Resource& operator=(Resource&& move) = default;

    const ResourceID&       GetID() const;
    TypeID                  GetTypeID() const;
    const std::string&      GetPath() const;

private:
    ResourceID              id;
    TypeID                  typeID;
    std::string             path;
};
