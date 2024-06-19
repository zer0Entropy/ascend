#pragma once

#include <SFML/Graphics/Rect.hpp>
#include "menu.hpp"
#include "optionSelector.hpp"
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
    struct Layer {
        enum class TypeID {
            Music,
            Decoration,
            MenuButton,
            MenuLabel
        };

        int                                 index;
        std::string                         name;
        TypeID                              typeID;

        std::vector<Entity>                 entities;
        
        std::vector<ResourceToken>          fonts;
        std::vector<ResourceToken>          textures;
        std::vector<RepeatingTextureToken>  repeatingTextures;
        std::vector<CompositeTextureToken>  compositeTextures;
        std::vector<ResourceToken>          musics;
        std::vector<ResourceToken>          sounds;

        std::vector<sf::Rect<unsigned int>> boundingBoxes;
        std::vector<Resource*>              spriteAttachments;
        std::vector<Resource*>              textAttachments;
        std::vector<Resource*>              musicAttachments;
        std::vector<std::string>            textContents;
        std::vector<Text::FontParameters>   fontParameters;
        std::vector<Alignment>              labelAlignments;
        std::vector<
            std::pair<  Event::TypeID,
                        ResourceID>>        textureSwitchTriggers;
    };

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
    void                                    LoadLayerIndex(const nlohmann::json& json);
    void                                    LoadLayer(const nlohmann::json& json);

    void                                    LoadResourcesJSON(const nlohmann::json& json, Layer& layer);
    void                                    LoadFontsJSON(const nlohmann::json& json, Layer& layer);
    void                                    LoadTexturesJSON(const nlohmann::json& json, Layer& layer);
    void                                    LoadMusicsJSON(const nlohmann::json& json, Layer& layer);
    void                                    LoadSoundsJSON(const nlohmann::json& json, Layer& layer);
    
    sf::IntRect                             LoadRectFromJSON(const nlohmann::json& json) const;
    void                                    LoadSimpleTextureJSON(const nlohmann::json& json, Layer& layer);
    void                                    LoadCompositeTextureJSON(const nlohmann::json& json, Layer& layer);
    void                                    LoadRepeatingTextureJSON(const nlohmann::json& json, Layer& layer);

    void                                    LoadResources(Layer& layer);
    void                                    LoadBoundingBoxes(const nlohmann::json& json, Layer& layer);
    void                                    LoadSprites(const nlohmann::json& json, Layer& layer);
    void                                    LoadTexts(const nlohmann::json& json, Layer& layer);
    void                                    LoadTextureSwitches(const nlohmann::json& json, Layer& layer);
    void                                    LoadLabelAlignments(const nlohmann::json& json, Layer& layer);
    void                                    LoadMusic(const nlohmann::json& json, Layer& layer);
    void                                    LoadSoundEffects(const nlohmann::json& json, Layer& layer);

    void                                    CreateDecorations(Layer& layer);
    void                                    CreateMenuButtons(Layer& layer);
    void                                    CreateMenuLabels(Layer& layer);

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