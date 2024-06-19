#pragma once

#include <nlohmann/json.hpp>
#include "resource.hpp"

class JSONDocument: public Resource {
public:
    JSONDocument(const ResourceID& resID, std::string_view resPath);

    const nlohmann::json&   GetJSON() const;
    void                    SetJSON(const nlohmann::json& copyJSON);
private:
    nlohmann::json          json;
};