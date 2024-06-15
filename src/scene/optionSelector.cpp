#include "../include/scene/optionSelector.hpp"
#include "../include/scene/scene.hpp"

MenuOptionSelector::MenuOptionSelector(Scene* parent):
    IEventSubscriber{},
    parentScene{parent} {

}

void MenuOptionSelector::HandleEvent(const Event& event) {
    if(event.GetTypeID() == Event::TypeID::CursorHoveringStarted) {
        parentScene->SetSelectedMenuOption((int)event.GetTarget());
    }
    else if(event.GetTypeID() == Event::TypeID::CursorHoveringStopped) {
        parentScene->SetSelectedMenuOption(-1);
    }
    else if(event.GetTypeID() == Event::TypeID::ButtonPressStarted) {
        
    }
    else if(event.GetTypeID() == Event::TypeID::ButtonPressCompleted) {
        parentScene->ConfirmSelectedMenuOption();
    }
    else if(event.GetTypeID() == Event::TypeID::ButtonPressAborted) {
        parentScene->SetSelectedMenuOption(-1);
    }
}