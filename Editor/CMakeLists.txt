cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

set(EDITOR_SRCS src/Editor.cpp src/GameDll.cpp src/GameProject.cpp src/EditorUi.cpp)
set(EDITOR_HDRS	src/GameDll.h src/GameProject.h src/EditorUi.h src/ScenePlayer.h src/EditorStyles.h src/EditorCamera.h)

add_executable(Editor ${EDITOR_SRCS} ${EDITOR_HDRS} ../dependencies/glad/src/gl.c)

target_compile_definitions(Editor PRIVATE CMAKEDIR="${CMAKE_BINARY_DIR}" GAMEDIR="${CMAKE_SOURCE_DIR}/Game" SOURCE="${CMAKE_SOURCE_DIR}")

target_include_directories(Editor PRIVATE ../Engine/Include )
target_link_libraries(Editor PRIVATE EnTT::EnTT glfw imgui::imgui Engine glm::glm)
target_include_directories(Editor PRIVATE ../dependencies/entt/single_include ../dependencies/glad/include)