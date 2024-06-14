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
        Texture,
        TextFile,
    };

    Resource(const ResourceID& resID, TypeID resType, std::string_view resPath);
    virtual ~Resource() = default;

    const ResourceID&       GetID() const;
    TypeID                  GetTypeID() const;
    const std::string&      GetPath() const;

private:
    ResourceID              id;
    TypeID                  typeID;
    std::string             path;
};
