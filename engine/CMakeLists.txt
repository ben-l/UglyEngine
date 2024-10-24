project(Engine)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glad)
#add_subdirectory(vendor/yaml-cpp)

file(GLOB
	SOURCES CONFIGURE_DEPENDS
	core/*.cpp
	imgui/*.cpp
	vendor/imgui/*.cpp
    vendor/stb_image/*.cpp
    vendor/yaml-cpp/src/*.cpp
	platform/linux/*.cpp
    platform/opengl/*.cpp
	renderer/*.cpp
    scene/*.cpp
)

add_library(test STATIC ${SOURCES})

target_precompile_headers(test PRIVATE core/uepch.h vendor/entt/include/entt.hpp)

target_compile_definitions(test PRIVATE 
        GLFW_INCLUDE_NONE 
        _CRT_SECURE_NO_WARNINGS
)
