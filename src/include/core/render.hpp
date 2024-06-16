#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "../component/renderable.hpp"
#include "../component/label.hpp"
#include "../component/alignLabel.hpp"
#include "../component/scale.hpp"
#include "../interface/system.hpp"

class RenderSystem: public ISystem {
public:
    RenderSystem(sf::RenderWindow& win);

    void                                        Update() override;
    ISystem::SystemID                           GetSystemID() const override;

    RenderableMgr&                              GetRenderableMgr() const;
    LabelMgr&                                   GetLabelMgr() const;
    AlignLabelMgr&                              GetAlignLabelMgr() const;
    ScaleRenderableMgr&                         GetScaleRenderableMgr() const;

private:
    sf::RenderWindow&                           window;
    RenderableMgr                               renderableMgr;
    LabelMgr                                    labelMgr;
    AlignLabelMgr                               alignLabelMgr;
    ScaleRenderableMgr                          scaleRenderableMgr;
};