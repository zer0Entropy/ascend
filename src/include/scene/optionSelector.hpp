#pragma once

#include "../interface/subscriber.hpp"

class Scene;

class MenuOptionSelector: public IEventSubscriber {
public:
    MenuOptionSelector(Scene* parent);

    void            HandleEvent(const Event& event) override;
private:
    Scene*          parentScene;
};