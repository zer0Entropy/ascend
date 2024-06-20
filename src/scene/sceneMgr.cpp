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
    Event::TypeID           typeID{event.GetTypeID()};
    if(typeID == Event::TypeID::QuitGameStarted) {
        this->ThrowSignal(SignalID::UserQuitGame);
    }
    else if(typeID == Event::TypeID::OptionsStarted) {
        ResourceID          sceneID{"OptionsScene"};
        std::string         scenePath{"/home/zeroc00l/Code/ascend/data/scene/options.json"};
        Push(sceneID, scenePath);
    }
}

Scene* SceneMgr::GetCurrentScene() const {
    if(stack.empty()) {
        return nullptr;
    }
    return stack.top().get();
}

void SceneMgr::PauseCurrentScene() {
    Scene*                  currentScene{GetCurrentScene()};
    if(!currentScene) {
        return;
    }
    EventSystem&            eventSystem{*Application::GetInstance().GetEventSystem()};
    eventSystem.UnsubscribeAll(&currentScene->GetOptionSelector());

    const auto&             hoverables{currentScene->GetHoverableMgr().GetList()};
    for(auto hoverable : hoverables) {
        hoverable->SetEnabled(false);
    }
    const auto&             textureSwitchers{currentScene->GetTextureSwitcherMgr().GetList()};
    for(auto textureSwitcher : textureSwitchers) {
        textureSwitcher->SetEnabled(false);
        eventSystem.UnsubscribeAll(textureSwitcher);
    }
    const auto&             leftClickables{currentScene->GetLeftClickableMgr().GetList()};
    for(auto leftClickable : leftClickables) {
        leftClickable->SetEnabled(false);
    }
}

std::unique_ptr<Scene> SceneMgr::Pop() {
    auto                        scene{std::move(stack.top())};
    stack.pop();
    return                      std::move(scene);
}

void SceneMgr::Push(const ResourceID& sceneDocID, std::string_view sceneDocPath) {
    Scene*                      currentScene{GetCurrentScene()};
    if(currentScene) {
        PauseCurrentScene();
    }

    int                         sceneIndex{(int)stack.size()};
    stack.push(std::make_unique<Scene>(sceneIndex, entityMgr, resourceMgr));
    currentScene = GetCurrentScene();

    JSONDocument*               jsonDocument{resourceMgr.LoadJSONDocument(sceneDocID, sceneDocPath)};
    if(jsonDocument) {
        currentScene->LoadFromJSON(jsonDocument->GetJSON());
    }
}
