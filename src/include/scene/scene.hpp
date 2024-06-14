#pragma once

#include "../component/boundingBox.hpp"
#include "../component/sprite.hpp"
#include "../component/text.hpp"
#include "../component/hoverable.hpp"
#include "../component/textureSwitcher.hpp"
#include "../interface/serialize.hpp"

class Scene: public ISerializeable {
public:
    Scene();
    Scene(const Scene& copy) = default;
    Scene(Scene&& move) = default;
    ~Scene() = default;
    Scene& operator=(const Scene& copy) = default;
    Scene& operator=(Scene&& move) = default;

    void                                    LoadFromJSON(const nlohmann::json& json) override;
    nlohmann::json                          SaveToJSON() const override;
    SpriteMgr&                              GetSpriteMgr() const;
    TextMgr&                                GetTextMgr() const;
    BoundingBoxMgr&                         GetBoundingBoxMgr() const;
    HoverableMgr&                           GetHoverableMgr() const;
    TextureSwitcherMgr&                     GetTextureSwitcherMgr() const;
    
    const std::vector<Entity>&              GetEntityList() const;

private:
    SpriteMgr                               spriteMgr;
    TextMgr                                 textMgr;
    BoundingBoxMgr                          boundingBoxMgr;
    HoverableMgr                            hoverableMgr;
    TextureSwitcherMgr                      textureSwitcherMgr;
    
    std::vector<Entity>                     entityList;
};