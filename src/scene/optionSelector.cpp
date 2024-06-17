#include "../include/scene/optionSelector.hpp"
#include "../include/scene/scene.hpp"

MenuOptionSelector::MenuOptionSelector(Scene* parent):
    IEventSubscriber{},
    parentScene{parent} {

}

void MenuOptionSelector::HandleEvent(const Event& event) {
    if(event.GetTypeID() == Event::TypeID::CursorHoveringStarted) {
        if(parentScene->GetMenu().options.empty()) {
            parentScene->SetSelectedMenuOption(-1);    
        }
        else {
            int menuIndex = (int)event.GetTarget() - (int)parentScene->GetMenu().options.at(0).widget;
            parentScene->SetSelectedMenuOption(menuIndex);
        }
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