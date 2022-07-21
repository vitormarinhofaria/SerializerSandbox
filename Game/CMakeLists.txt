cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

add_library(Game SHARED src/main.cpp Include/Scripts.h)

target_include_directories(Game PRIVATE ../Engine/Include)
target_link_directories(Game PRIVATE ../Engine/build)
target_link_libraries(Game Engine)

target_include_directories(Game PRIVATE ../dependencies/entt/single_include Include)
