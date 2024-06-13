#include "../include/core/app.hpp"

Application* Application::instance{nullptr};

EntityMgr& Application::GetEntityMgr() const {
    return const_cast<EntityMgr&>(entityMgr);
}

ResourceMgr& Application::GetResourceMgr() const {
    return const_cast<ResourceMgr&>(resourceMgr);
}

LogSystem* Application::GetLogSystem() const {
    return logSystem.get();
}

InputSystem* Application::GetInputSystem() const {
    return inputSystem.get();
}

RenderSystem* Application::GetRenderSystem() const {
    return renderSystem.get();
}

sf::RenderWindow& Application::GetWindow() const {
    return const_cast<sf::RenderWindow&>(window);
}

Application::StatusID Application::GetStatus() const {
    if(     Application::instance
        &&  window.isOpen()) {
        return Application::StatusID::Running;
    }
    return Application::StatusID::Stopped;
}

void Application::Update() {
    if(this->GetStatus() == StatusID::Running) {
        window.clear();

        for(auto system : systemList) {
            system->Update();
        }

        window.display();
    }
}

Application::Application() {
    for(int index = 0; index < systemList.size(); ++index) {
        systemList[index] = nullptr;
    }
}

void Application::Start() {
    logSystem = std::make_unique<LogSystem>();
    inputSystem = std::make_unique<InputSystem>(window);
    renderSystem = std::make_unique<RenderSystem>(window);
    systemList[(int)ISystem::SystemID::LogSystem] = logSystem.get();
    systemList[(int)ISystem::SystemID::InputSystem] = inputSystem.get();
    systemList[(int)ISystem::SystemID::RenderSystem] = renderSystem.get();
    
    sf::Vector2u                windowSize{1920, 1080};
    std::string                 windowTitle{"ASCEND"};
    window.create(sf::VideoMode(windowSize.x, windowSize.y), windowTitle, sf::Style::Fullscreen);

    sceneMgr.Push(std::make_unique<Scene>());
}

void Application::Stop() {
    if(window.isOpen()) {
        window.close();
    }
    for(int index = 0; index < NumSystems; ++index) {
        systemList[index] = nullptr;
    }
    renderSystem.reset(nullptr);
    inputSystem.reset(nullptr);
    logSystem.reset(nullptr);
}