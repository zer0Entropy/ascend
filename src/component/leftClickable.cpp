#include <SFML/Window/Event.hpp>
#include "../include/component/leftClickable.hpp"
#include "../include/core/event.hpp"
#include "../include/core/app.hpp"

LeftClickable::LeftClickable(Entity cEntity, const BoundingBox& boundsCmp, EventSystem& eventSys):
    Component{Component::TypeID::LeftClickable, cEntity},
    IInputListener{},
    eventSystem{eventSys},
    boundingBox{boundsCmp} {

}

void LeftClickable::LoadFromJSON(const nlohmann::json& json) {

}

nlohmann::json LeftClickable::SaveToJSON() const {
    return nlohmann::json{};
}

bool LeftClickable::ReceiveInput(const sf::Event& event) {
    bool handled{false};
    if(     event.type == sf::Event::MouseButtonPressed
        &&  event.mouseButton.button == sf::Mouse::Button::Left) {
        sf::Vector2u cursorPos{(unsigned int)event.mouseButton.x, (unsigned int)event.mouseButton.y};
        if(IsInBounds(cursorPos, boundingBox.GetRect())) {
            if(!isPressedDown) {
                isPressedDown = true;
                eventSystem.Enqueue(Event{Event::TypeID::ButtonPressStarted, this->GetOwner()});
            }
            handled = true;
        }
        else {
            if(isPressedDown) {
                isPressedDown = false;
                eventSystem.Enqueue(Event{Event::TypeID::ButtonPressAborted, this->GetOwner()});
            }
        }
    }
    else if(    event.type == sf::Event::MouseButtonReleased
            &&  event.mouseButton.button == sf::Mouse::Button::Left) {
        sf::Vector2u cursorPos{(unsigned int)event.mouseButton.x, (unsigned int)event.mouseButton.y};
        if(IsInBounds(cursorPos, boundingBox.GetRect())) {
            if(isPressedDown) {
                isPressedDown = false;
                eventSystem.Enqueue(Event{Event::TypeID::ButtonPressCompleted, this->GetOwner()});
            }
        }
        else if(isPressedDown) {
            isPressedDown = false;
        }
    }
    return handled;
}

bool LeftClickable::IsPressedDown() const {
    return isPressedDown;
}

void LeftClickable::SetPressedDown(bool pressedDown) {
    isPressedDown = pressedDown;
}

void LeftClickableMgr::Add(Entity owner, BoundingBox& boundsCmp) {
    lClickMap.insert(
        std::make_pair( owner,
                        std::make_unique<LeftClickable>(owner, boundsCmp, *Application::GetInstance().GetEventSystem())));
}

void LeftClickableMgr::Remove(Entity owner) {
    const auto& iter{lClickMap.find(owner)};
    if(iter != lClickMap.end()) {
        lClickMap.erase(iter);
    }
}

LeftClickable* LeftClickableMgr::Get(Entity owner) {
    const auto& iter{lClickMap.find(owner)};
    if(iter != lClickMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<LeftClickable*> LeftClickableMgr::GetList() const {
    std::vector<LeftClickable*> lClickList;
    for(const auto& leftClickable : lClickMap) {
        lClickList.push_back(leftClickable.second.get());
    }
    return lClickList;
}