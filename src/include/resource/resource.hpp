#pragma once

#include <string>
#include "../interface/serialize.hpp"

using ResourceID = std::string;

class Resource: public ISerializeable {
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

    void                    LoadFromJSON(const nlohmann::json& json) override;
    nlohmann::json          SaveToJSON() const override;

private:
    ResourceID              id;
    TypeID                  typeID;
    std::string             path;
};
