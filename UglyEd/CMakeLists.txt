project(UglyEd)

# copies assets dir from sandbox to the build root directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

file(GLOB
	SOURCES CONFIGURE_DEPENDS
	src/*.cpp
	src/Panels/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE test glfw glad)
