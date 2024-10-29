project(Engine)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glad)
#add_subdirectory(vendor/yaml-cpp)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

file(GLOB
	SOURCES CONFIGURE_DEPENDS
	core/*.cpp
	imgui/*.cpp
    maths/*.cpp
	vendor/imgui/*.cpp
    vendor/stb_image/*.cpp
    vendor/yaml-cpp/src/*.cpp
    vendor/imguizmo/*.cpp
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
target_include_directories(test PRIVATE ${GTK_INCLUDE_DIRS})
target_link_directories(test PRIVATE ${GTK_LIBRARY_DIRS})
target_compile_options(test PRIVATE ${GTK_CFLAGS_OTHER})

target_link_libraries(test PRIVATE ${GTK_LIBRARIES})
