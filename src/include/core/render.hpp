#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "../component/layerIndex.hpp"
#include "../component/renderable.hpp"
#include "../component/label.hpp"
#include "../component/alignable.hpp"
#include "../component/scale.hpp"
#include "../interface/system.hpp"

class RenderSystem: public ISystem {
public:
    RenderSystem(sf::RenderWindow& win);

    void                                        Update() override;
    ISystem::SystemID                           GetSystemID() const override;

    LayerIndexMgr&                              GetLayerIndexMgr() const;
    RenderableMgr&                              GetRenderableMgr() const;
    LabelMgr&                                   GetLabelMgr() const;
    AlignableMgr&                               GetAlignableMgr() const;
    ScaleRenderableMgr&                         GetScaleRenderableMgr() const;

private:
    sf::RenderWindow&                           window;
    LayerIndexMgr                               layerIndexMgr;
    RenderableMgr                               renderableMgr;
    LabelMgr                                    labelMgr;
    AlignableMgr                                alignableMgr;
    ScaleRenderableMgr                          scaleRenderableMgr;
};