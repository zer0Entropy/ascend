#include <fstream>
#include "../include/scene/sceneMgr.hpp"
#include "../include/core/app.hpp"

SceneMgr::SceneMgr(EntityMgr& entMgr, ResourceMgr &resMgr):
    IEventSubscriber{},
    IThrowSignal{},
    entityMgr{entMgr},
    resourceMgr{resMgr} {

}

void SceneMgr::HandleEvent(const Event& event) {
    if(event.GetTypeID() == Event::TypeID::QuitGameStarted) {
        this->ThrowSignal(SignalID::UserQuitGame);
    }
}

Scene* SceneMgr::GetCurrentScene() const {
    if(stack.empty()) {
        return nullptr;
    }
    return stack.top().get();
}

std::unique_ptr<Scene> SceneMgr::Pop() {
    auto                        scene{std::move(stack.top())};
    stack.pop();
    return                      std::move(scene);
}

void SceneMgr::Push(const ResourceID& sceneDocID, std::string_view sceneDocPath) {
    stack.push(std::make_unique<Scene>(entityMgr, resourceMgr));

    JSONDocument*               jsonDocument{resourceMgr.LoadJSONDocument(sceneDocID, sceneDocPath)};
    if(jsonDocument) {
        Scene*                      currentScene{GetCurrentScene()};
        currentScene->LoadFromJSON(jsonDocument->GetJSON());
    }
}
