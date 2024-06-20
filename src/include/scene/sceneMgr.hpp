#pragma once

#include <stack>
#include "scene.hpp"
#include "../interface/signal.hpp"
#include "../interface/subscriber.hpp"

class EntityMgr;
class ResourceMgr;

class SceneMgr: public IEventSubscriber, public IThrowSignal {
public:
    SceneMgr(EntityMgr& entMgr, ResourceMgr& resMgr);

    void                                    HandleEvent(const Event& event) override;

    Scene*                                  GetCurrentScene() const;

    void                                    PauseCurrentScene();

    std::unique_ptr<Scene>                  Pop();
    void                                    Push(const ResourceID& sceneDocID, std::string_view sceneDocPath);
private:
    EntityMgr&                              entityMgr;
    ResourceMgr&                            resourceMgr;
    std::stack<std::unique_ptr<Scene>>      stack;
};