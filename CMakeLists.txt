cmake_minimum_required(VERSION 3.19.2)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_BUILD_PARALLEL_LEVEL 8)

project(UglyEngine)

include_directories(
    engine/core
    engine/events
    engine/imgui
    engine/renderer
    engine/scene
    engine/vendor/glfw/include
    engine/vendor/glad/include
    engine/vendor/spdlog/include
    engine/vendor/imgui
    engine/vendor/imgui/backends
    engine/vendor/glm/glm
    engine/vendor/stb_image
    engine/vendor/entt/include
    engine/vendor/yaml-cpp/include
    engine/platform/linux
    engine/platform/opengl
    engine/debug
    sandbox/src
    UglyEd/src
    UglyEd/src/Panels
)

add_subdirectory(engine)
add_subdirectory(sandbox)
add_subdirectory(UglyEd)
