#pragma once

#include "menu.hpp"
#include "../component/boundingBox.hpp"
#include "../component/sprite.hpp"
#include "../component/text.hpp"
#include "../component/hoverable.hpp"
#include "../component/textureSwitcher.hpp"
#include "../interface/serialize.hpp"
#include "../interface/publisher.hpp"
#include "../resource/resourceMgr.hpp"

class Scene: public ISerializeable, public ILogMsgPublisher {
public:
    Scene(EntityMgr& entMgr, ResourceMgr& resMgr);
    Scene(const Scene& copy) = delete;
    Scene(Scene&& move) = delete;
    ~Scene() = default;
    Scene& operator=(const Scene& copy) = delete;
    Scene& operator=(Scene&& move) = delete;

    void                                    LoadFromJSON(const nlohmann::json& json) override;
    nlohmann::json                          SaveToJSON() const override;
    
    SpriteMgr&                              GetSpriteMgr() const;
    TextMgr&                                GetTextMgr() const;
    BoundingBoxMgr&                         GetBoundingBoxMgr() const;
    HoverableMgr&                           GetHoverableMgr() const;
    TextureSwitcherMgr&                     GetTextureSwitcherMgr() const;

    const Menu&                             GetMenu() const;

    const std::vector<ResourceToken>&       GetFontList() const;
    const std::vector<ResourceToken>&       GetTextureList() const;

private:
    void                                    LoadResourceList(const nlohmann::json& json);
    void                                    LoadSprite(Entity owner, const nlohmann::json& json);
    void                                    LoadText(Entity owner, const nlohmann::json& json);
    void                                    LoadBoundingBox(Entity owner, const nlohmann::json& json);
    Alignment                               LoadAlignLabel(const nlohmann::json& json) const;
    void                                    LoadTextureSwitches(const nlohmann::json& json);

    SpriteMgr                               spriteMgr;
    TextMgr                                 textMgr;
    BoundingBoxMgr                          boundingBoxMgr;
    HoverableMgr                            hoverableMgr;
    TextureSwitcherMgr                      textureSwitcherMgr;
    
    EntityMgr&                              entityMgr;
    ResourceMgr&                            resourceMgr;

    std::vector<ResourceToken>              fontList;
    std::vector<ResourceToken>              textureList;

    Menu                                    menu;
};