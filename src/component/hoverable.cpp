#include <SFML/Window/Event.hpp>
#include "../include/component/hoverable.hpp"
#include "../include/core/event.hpp"
#include "../include/core/app.hpp"

Hoverable::Hoverable(Entity cEntity, const BoundingBox& boundsCmp, EventSystem& eventSys):
    Component{Component::TypeID::Hoverable, cEntity},
    boundingBox{boundsCmp},
    eventSystem{eventSys},
    status{HoverStatus::Off} {

}

void Hoverable::LoadFromJSON(const nlohmann::json& json) {

}

nlohmann::json Hoverable::SaveToJSON() const {
    return nlohmann::json{};
}

bool Hoverable::ReceiveInput(const sf::Event& event) {
    bool handled{false};
    if(event.type == sf::Event::MouseMoved) {
        sf::Vector2u cursorPos{(unsigned int)event.mouseMove.x, (unsigned int)event.mouseMove.y};
        if(IsInBounds(cursorPos, boundingBox.GetRect())) {
            if(status == HoverStatus::Off) {
                status = HoverStatus::On;
                eventSystem.Enqueue(Event{Event::TypeID::CursorHoveringStarted, this->GetOwner()});
            }
            handled = true;
        }
        else {
            if(status == HoverStatus::On) {
                status = HoverStatus::Off;
                eventSystem.Enqueue(Event{Event::TypeID::CursorHoveringStopped, this->GetOwner()});
            }
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased) {
        status = HoverStatus::Off;
    }
    return handled;
}

Hoverable::HoverStatus Hoverable::GetHoverStatus() const {
    return status;
}

void HoverableMgr::Add(Entity owner, BoundingBox& boundsCmp) {
    hoverableMap.insert(
        std::make_pair(
            owner,
            std::make_unique<Hoverable>(owner, boundsCmp, *Application::GetInstance().GetEventSystem())));
}

void HoverableMgr::Remove(Entity owner) {
    const auto& iter{hoverableMap.find(owner)};
    if(iter != hoverableMap.end()) {
        hoverableMap.erase(iter);
    }
}

Hoverable* HoverableMgr::Get(Entity owner) {
    const auto& iter{hoverableMap.find(owner)};
    if(iter != hoverableMap.end() && (*iter).second->IsEnabled()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<Hoverable*> HoverableMgr::GetList() const {
    std::vector<Hoverable*> hoverableList;
    for(const auto& hoverable : hoverableMap) {
        if(hoverable.second->IsEnabled()) {
            hoverableList.push_back(hoverable.second.get());
        }
    }
    return hoverableList;
}