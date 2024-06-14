#include <fstream>
#include "../include/scene/sceneMgr.hpp"
#include "../include/core/app.hpp"

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

void SceneMgr::Push(std::unique_ptr<Scene> scene) {
    stack.push(std::move(scene));

    std::string                 scenePath{"/home/zeroc00l/Code/ascend/data/scene/titleScene.json"};
    nlohmann::json              sceneJSON;
    std::ifstream               sceneFile{scenePath, std::ios_base::in};
    sceneFile >> sceneJSON;
    Scene*                      currentScene{GetCurrentScene()};
    currentScene->LoadFromJSON(sceneJSON);

    HoverableMgr&               hoverableMgr{currentScene->GetHoverableMgr()};
    TextureSwitcherMgr&         textureSwitcherMgr{currentScene->GetTextureSwitcherMgr()};
    InputSystem&                inputSystem{*Application::GetInstance().GetInputSystem()};
    EventSystem&                eventSystem{*Application::GetInstance().GetEventSystem()};
    ResourceMgr&                resourceMgr{Application::GetInstance().GetResourceMgr()};
    ResourceID                  normalTextureID{"ButtonTexture"};
    ResourceID                  hoverTextureID{"ButtonTextureHover"};
    std::string                 hoverTexturePath{"/home/zeroc00l/Code/ascend/data/img/uiTextures/Orange/Btn_over.png"};
    resourceMgr.LoadTexture(hoverTextureID, hoverTexturePath);
    Texture*                    normalTexture{resourceMgr.GetTexture(normalTextureID)};
    Texture*                    hoverTexture{resourceMgr.GetTexture(hoverTextureID)};
    for(const auto entity : currentScene->GetEntityList()) {
        hoverableMgr.Add(entity, *currentScene->GetBoundingBoxMgr().Get(entity));
        textureSwitcherMgr.Add(entity, *currentScene->GetSpriteMgr().Get(entity));
        auto hoverable{hoverableMgr.Get(entity)};
        auto textureSwitcher(textureSwitcherMgr.Get(entity));
        inputSystem.Subscribe(hoverable);
        textureSwitcher->AddTrigger(Event::TypeID::CursorHoveringStarted, hoverTextureID);
        textureSwitcher->AddTrigger(Event::TypeID::CursorHoveringStopped, normalTextureID);
        textureSwitcher->Attach(normalTexture);
        textureSwitcher->Attach(hoverTexture);
        eventSystem.Subscribe(textureSwitcher, Event::TypeID::CursorHoveringStarted);
        eventSystem.Subscribe(textureSwitcher, Event::TypeID::CursorHoveringStopped);
    }
}