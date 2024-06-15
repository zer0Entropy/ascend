cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(ascend VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(ascend       src/main.cpp
                            src/core/app.cpp
                            src/core/common.cpp
                            src/core/event.cpp
                            src/core/input.cpp
                            src/core/inputHandler.cpp
                            src/core/log.cpp
                            src/core/render.cpp
                            src/component/boundingBox.cpp
                            src/component/alignLabel.cpp
                            src/component/label.cpp
                            src/component/renderable.cpp
                            src/component/sprite.cpp
                            src/component/text.cpp
                            src/component/hoverable.cpp
                            src/component/textureSwitcher.cpp
                            src/component/leftClickable.cpp
                            src/resource/resource.cpp
                            src/resource/textFile.cpp
                            src/resource/texture.cpp
                            src/resource/font.cpp
                            src/resource/resourceMgr.cpp
                            src/scene/scene.cpp
                            src/scene/sceneMgr.cpp
                            src/scene/optionSelector.cpp
                            src/interface/signal.cpp)

#target_compile_options(ascend PRIVATE -fstandalone-debug)

target_link_libraries(ascend PRIVATE sfml-audio sfml-window sfml-graphics sfml-system nlohmann_json::nlohmann_json)
