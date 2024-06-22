#include <algorithm>
#include "../include/core/render.hpp"
#include "../include/component/sprite.hpp"
#include "../include/component/text.hpp"

RenderSystem::RenderSystem(sf::RenderWindow& win):
    ISystem{},
    window{win} {

}

void RenderSystem::Update() {
    const auto& renderList{renderableMgr.GetList()};
    for(const auto& renderable : renderList) {
        Sprite*             sprite{renderable->GetSprite()};
        Text*               text{renderable->GetText()};
        Entity              owner{renderable->GetOwner()};
        auto                scale{scaleRenderableMgr.Get(owner)};
        auto                alignable{alignableMgr.Get(owner)};
        if(scale) {
            const auto&     scaleFactor{scale->GetScalingFactor()};
            if(sprite) {
                sprite->GetSprite().setScale(scaleFactor);
            }
            if(text) {
                text->GetText().setScale(scaleFactor);
            }
        }
        const auto&         bounds{renderable->GetBounds()};
        if(sprite) {
            sf::Sprite&     sfmlSprite{sprite->GetSprite()};
            if(alignable) {
                const auto& alignBounds{alignable->GetTargetBoundingBox()};
                const auto& alignRect{alignBounds.GetRect()};
                Alignment   alignment{alignable->GetAlignment()};
                switch(alignment) {
                    case Alignment::Left:
                        sfmlSprite.setPosition(alignRect.left, alignRect.top);
                        break;
                    case Alignment::Center:
                        sfmlSprite.setPosition(
                            alignRect.left + (alignRect.width / 2.0f) - (sfmlSprite.getGlobalBounds().width / 2.0f),
                            alignRect.top + (alignRect.height / 2.0f) - (sfmlSprite.getGlobalBounds().height / 2.0f));
                        break;
                    case Alignment::Right:
                        sfmlSprite.setPosition(
                            alignRect.left + alignRect.width - sfmlSprite.getGlobalBounds().width,
                            alignRect.top  
                        );
                        break;
                }
            }
            else {
                sfmlSprite.setPosition(bounds.GetLeft(), bounds.GetTop());
            }
            window.draw(sfmlSprite);
        }
        if(text) {
            sf::Text&       sfmlText{text->GetText()};
            const auto&     alignBounds{alignable->GetTargetBoundingBox()};
            const auto&     alignRect{alignBounds.GetRect()};
            Alignment       alignment{alignable->GetAlignment()};
            if(alignable) {
                switch(alignment) {
                    case Alignment::Left:
                        sfmlText.setPosition(alignRect.left, alignRect.top);
                    break;
                    case Alignment::Center:
                        sfmlText.setPosition(
                            alignRect.left + (alignRect.width / 2.0f) - (sfmlText.getGlobalBounds().width / 2.0f),
                            alignRect.top + (alignRect.height / 5.0f) + (sfmlText.getCharacterSize() / 10.0f));
                    break;
                    case Alignment::Right:
                        sfmlText.setPosition(
                            alignRect.left + alignRect.width - sfmlText.getGlobalBounds().width,
                            alignRect.top);
                    break;
                }
            }
            else {
                sfmlText.setPosition(bounds.GetLeft(), bounds.GetTop());
            }
            window.draw(sfmlText);
        }
    }
}

ISystem::SystemID RenderSystem::GetSystemID() const {
    return ISystem::SystemID::RenderSystem;
}

LayerIndexMgr& RenderSystem::GetLayerIndexMgr() const {
    return const_cast<LayerIndexMgr&>(layerIndexMgr);
}

RenderableMgr& RenderSystem::GetRenderableMgr() const {
    return const_cast<RenderableMgr&>(renderableMgr);
}

LabelMgr& RenderSystem::GetLabelMgr() const {
    return const_cast<LabelMgr&>(labelMgr);
}

AlignableMgr& RenderSystem::GetAlignableMgr() const {
    return const_cast<AlignableMgr&>(alignableMgr);
}

ScaleRenderableMgr& RenderSystem::GetScaleRenderableMgr() const {
    return const_cast<ScaleRenderableMgr&>(scaleRenderableMgr);
}