#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "../component/renderable.hpp"
#include "../component/label.hpp"
#include "../component/alignLabel.hpp"
#include "../interface/system.hpp"

class RenderSystem: public ISystem {
public:
    RenderSystem(sf::RenderWindow& win);

    void                                        Update();
    ISystem::SystemID                           GetSystemID() const;

    RenderableMgr&                              GetRenderableMgr() const;
    LabelMgr&                                   GetLabelMgr() const;
    AlignLabelMgr&                              GetAlignLabelMgr() const;

private:
    sf::RenderWindow&                           window;
    RenderableMgr                               renderableMgr;
    LabelMgr                                    labelMgr;
    AlignLabelMgr                               alignLabelMgr;
};