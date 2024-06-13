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

    RenderSystem*               renderSystem{Application::GetInstance().GetRenderSystem()};
    RenderableMgr&              renderableMgr{renderSystem->GetRenderableMgr()};
    LabelMgr&                   labelMgr{renderSystem->GetLabelMgr()};
    AlignLabelMgr&              alignLabelMgr{renderSystem->GetAlignLabelMgr()};
    const auto&                 buttonList{currentScene->GetButtonList()};
    for(auto button : buttonList) {
        auto                    buttonBounds{currentScene->GetBoundingBoxMgr().Get(button)};
        auto                    buttonSprite{currentScene->GetSpriteMgr().Get(button)};
        auto                    buttonText{currentScene->GetTextMgr().Get(button)};
        renderableMgr.Add(button, *buttonBounds, *buttonSprite);
        labelMgr.Add(button, *buttonBounds, *buttonText);
        alignLabelMgr.Add(button, Alignment::Center, *labelMgr.Get(button));
    }
}