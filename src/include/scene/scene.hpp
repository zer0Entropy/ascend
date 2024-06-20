#pragma once

#include <SFML/Graphics/Rect.hpp>
#include "layer.hpp"
#include "menu.hpp"
#include "optionSelector.hpp"
#include "parser.hpp"
#include "../component/boundingBox.hpp"
#include "../component/sprite.hpp"
#include "../component/text.hpp"
#include "../component/hoverable.hpp"
#include "../component/textureSwitcher.hpp"
#include "../component/leftClickable.hpp"
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
    LeftClickableMgr&                       GetLeftClickableMgr() const;

    const Menu&                             GetMenu() const;
    void                                    SetSelectedMenuOption(int index);
    void                                    ConfirmSelectedMenuOption();

private:
    Layer*                                  GetLayer(std::string_view name);

    void                                    LoadResources(Layer& layer);

    void                                    CreateDecorations(Layer& layer);
    void                                    CreateMenuButtons(Layer& layer);
    void                                    CreateMenuLabels(Layer& layer);
    void                                    CreateSoundEffects(Layer& layer);
    void                                    PlayBackgroundMusic(Layer &layer);

    Entity                                  FindWidgetUnderCursor() const;

    SpriteMgr                               spriteMgr;
    TextMgr                                 textMgr;
    BoundingBoxMgr                          boundingBoxMgr;
    HoverableMgr                            hoverableMgr;
    TextureSwitcherMgr                      textureSwitcherMgr;
    LeftClickableMgr                        leftClickableMgr;
    
    EntityMgr&                              entityMgr;
    ResourceMgr&                            resourceMgr;

    std::vector<Layer>                      layers;

    Menu                                    menu;
    MenuOptionSelector                      optionSelector;
};