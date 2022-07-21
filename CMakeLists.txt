cmake_minimum_required(VERSION 3.22)
project(GameEngine)

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

add_subdirectory(Editor)
add_subdirectory(Engine)
#add_subdirectory(Game)
#add_subdirectory(dependencies/glfw)
#add_subdirectory(dependencies/imgui_cmake)
#add_subdirectory(dependencies/yaml-cpp)


