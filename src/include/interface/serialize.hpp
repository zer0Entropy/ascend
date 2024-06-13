#pragma once

#include <nlohmann/json.hpp>

class ISerializeable {
public:
    virtual ~ISerializeable() = default;

    virtual void                        LoadFromJSON(const nlohmann::json& json) = 0;

    virtual nlohmann::json              SaveToJSON() const = 0;
};