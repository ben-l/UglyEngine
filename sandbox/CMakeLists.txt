project(Sandbox)

# copies assets dir from sandbox to the build root directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

file(GLOB
	SOURCES CONFIGURE_DEPENDS
	src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
#add_executable(${PROJECT_NAME} src/SandboxApp.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE test glfw glad)
