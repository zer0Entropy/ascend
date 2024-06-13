#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include "entity.hpp"
#include "input.hpp"
#include "log.hpp"
#include "render.hpp"
#include "../resource/resourceMgr.hpp"
#include "../scene/sceneMgr.hpp"

class Application {
public:
    ~Application() = default;
    Application(const Application& copy) = delete;
    Application(Application&& move) = delete;
    Application& operator=(const Application& copy) = delete;
    Application& operator=(Application&& move) = delete;

    static Application&                     GetInstance() {
        if(!Application::instance) {
            Application::instance = new Application;
            Application::instance->Start();
        }
        return *Application::instance;
    }

    EntityMgr&                              GetEntityMgr() const;
    ResourceMgr&                            GetResourceMgr() const;

    LogSystem*                              GetLogSystem() const;
    InputSystem*                            GetInputSystem() const;
    RenderSystem*                           GetRenderSystem() const;
    
    sf::RenderWindow&                       GetWindow() const;

    enum class StatusID {
        Stopped,
        Running
    };
    StatusID                                GetStatus() const;

    void                                    Update();

private:
    Application();

    void                                    Start();
    void                                    Stop();

    static Application*                     instance;

    EntityMgr                               entityMgr;
    ResourceMgr                             resourceMgr;
    SceneMgr                                sceneMgr;

    std::array<ISystem*, NumSystems>        systemList;
    std::unique_ptr<LogSystem>              logSystem;
    std::unique_ptr<InputSystem>            inputSystem;
    std::unique_ptr<RenderSystem>           renderSystem;

    sf::RenderWindow                        window;
};