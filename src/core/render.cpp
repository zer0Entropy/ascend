#include "../include/core/render.hpp"

RenderSystem::RenderSystem(sf::RenderWindow& win):
    ISystem{},
    window{win} {

}

void RenderSystem::Update() {
    const auto renderableList{renderableMgr.GetList()};
    for(const auto& renderable : renderableList) {
        const auto& bounds{renderable->GetBounds()};
        auto& sprite{renderable->GetSprite()};
        auto scaleRenderable{scaleRenderableMgr.Get(sprite.GetOwner())};
        if(scaleRenderable) {
            const auto& scaleFactor{scaleRenderable->GetScalingFactor()};
            sprite.GetSprite().setScale(scaleFactor.x, scaleFactor.y);
        }
        sprite.GetSprite().setPosition(bounds.GetLeft(), bounds.GetTop());
        window.draw(sprite.GetSprite());
    }
    const auto labelList{labelMgr.GetList()};
    for(const auto& label : labelList) {
        const auto& bounds{label->GetBounds()};
        const auto& boundingBox{bounds.GetRect()};
        auto& text{label->GetText()};
        auto alignLabel{alignLabelMgr.Get(label->GetOwner())};
        sf::Text& sfmlText{text.GetText()};
        const sf::Vector2u textSize{
            (unsigned int)sfmlText.getGlobalBounds().width,
            (unsigned int)sfmlText.getGlobalBounds().height
        };
        if(alignLabel) {
            switch(alignLabel->GetAlignment()) {
                default:
                case Alignment::Left: {
                    sfmlText.setPosition(   boundingBox.left,
                                            boundingBox.top + boundingBox.height - sfmlText.getGlobalBounds().height);
                } break;
                case Alignment::Center: {
                    sfmlText.setPosition(   boundingBox.left + (boundingBox.width / 2.0f) - (textSize.x / 2.0f),
                                            boundingBox.top + (boundingBox.height / 2.0f) - (3 * text.GetFontParameters().fontSize / 5.0f));
                } break;
                case Alignment::Right: {
                    sfmlText.setPosition(   boundingBox.left + boundingBox.width - textSize.x,
                                            boundingBox.top + boundingBox.height - textSize.y);
                }
            }
        }
        else {
            sfmlText.setPosition(bounds.GetLeft(), bounds.GetTop());
        }
        window.draw(text.GetText());
    }
}

ISystem::SystemID RenderSystem::GetSystemID() const {
    return ISystem::SystemID::RenderSystem;
}

RenderableMgr& RenderSystem::GetRenderableMgr() const {
    return const_cast<RenderableMgr&>(renderableMgr);
}

LabelMgr& RenderSystem::GetLabelMgr() const {
    return const_cast<LabelMgr&>(labelMgr);
}

AlignLabelMgr& RenderSystem::GetAlignLabelMgr() const {
    return const_cast<AlignLabelMgr&>(alignLabelMgr);
}

ScaleRenderableMgr& RenderSystem::GetScaleRenderableMgr() const {
    return const_cast<ScaleRenderableMgr&>(scaleRenderableMgr);
}