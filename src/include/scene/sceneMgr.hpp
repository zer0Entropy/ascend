#pragma once

#include <stack>
#include "scene.hpp"

class SceneMgr {
public:
    Scene*                                  GetCurrentScene() const;
    std::unique_ptr<Scene>                  Pop();
    void                                    Push(std::unique_ptr<Scene> scene);
private:
    std::stack<std::unique_ptr<Scene>>      stack;
};