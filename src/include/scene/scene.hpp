#pragma once

#include "../component/boundingBox.hpp"
#include "../component/sprite.hpp"
#include "../component/text.hpp"
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
    const std::vector<Entity>&              GetButtonList() const;

private:
    SpriteMgr                               spriteMgr;
    TextMgr                                 textMgr;
    BoundingBoxMgr                          boundingBoxMgr;
    std::vector<Entity>                     buttons;
};