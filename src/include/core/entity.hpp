#pragma once

#include <cstdint>

using Entity = uint64_t;

class EntityMgr {
public:
    EntityMgr(): nextAvailable{0} {}
    EntityMgr(const EntityMgr& copy) = delete;
    EntityMgr(EntityMgr&& move) = delete;
    ~EntityMgr() = default;
    EntityMgr& operator=(const EntityMgr& copy) = delete;
    EntityMgr& operator=(EntityMgr&& move) = delete;

    Entity          CreateEntity() {
        return nextAvailable++;
    }

    bool            CheckIfExists(Entity entity) const {
        if(entity < nextAvailable) {
            return true;
        }
        return false;
    }
private:
    Entity nextAvailable;
};