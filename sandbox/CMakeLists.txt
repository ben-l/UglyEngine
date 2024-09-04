project(Sandbox)

# copies assets dir from sandbox to the build root directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} src/SandboxApp.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE test glfw glad)
