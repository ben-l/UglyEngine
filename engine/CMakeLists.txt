project(Engine)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glad)

file(GLOB
	SOURCES CONFIGURE_DEPENDS
	core/*.cpp
	imgui/*.cpp
	vendor/imgui/*.cpp
	platform/linux/*.cpp
        platform/opengl/*.cpp
	renderer/*.cpp
)

add_library(test STATIC ${SOURCES})

target_precompile_headers(test PRIVATE core/uepch.h)

target_compile_definitions(test PRIVATE 
        GLFW_INCLUDE_NONE 
        _CRT_SECURE_NO_WARNINGS
)
