#pragma once

#include <stack>
#include "scene.hpp"
#include "../interface/signal.hpp"
#include "../interface/subscriber.hpp"

class SceneMgr: public IEventSubscriber, public IThrowSignal {
public:
    void                                    HandleEvent(const Event& event) override;

    Scene*                                  GetCurrentScene() const;

    std::unique_ptr<Scene>                  Pop();
    void                                    Push(std::unique_ptr<Scene> scene);
private:
    std::stack<std::unique_ptr<Scene>>      stack;
};