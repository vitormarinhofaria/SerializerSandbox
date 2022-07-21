cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

find_package(EnTT CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

set(EngineHs Include/Common.h Include/Scene.h Include/Components.h  Include/SceneSerializer.h 
				Include/Entity.h Include/ScriptRegistry.h Include/Renderer.h Include/Buffer.h 
				Include/Shader.h Include/Texture.h Include/RenderData.h)

set(EngineSrcs src/Scene.cpp src/Components.cpp src/SceneSerializer.cpp 
				src/Entity.cpp src/ScriptRegistry.cpp src/Renderer.cpp src/Buffer.cpp
				src/Shader.cpp src/Texture.cpp src/RenderData.cpp)

add_library(Engine SHARED ${EngineHs} ${EngineSrcs})

file(GLOB EngineOpenGLSrcs src/Platform/Opengl/**.cpp src/Platform/Opengl/**.h)
target_sources(Engine PRIVATE ${EngineOpenGLSrcs} dependencies/glad/src/gl.c)
target_include_directories(Engine PRIVATE dependencies/glad/include)
target_compile_definitions(Engine PRIVATE PLATFORM_OPENGL)

if(WIN32)
target_compile_definitions(Engine PRIVATE PLATFORM_WINDOWS)
endif()

target_compile_definitions(Engine PRIVATE EXPORT)
target_include_directories(Engine PRIVATE ./Include)

target_link_libraries(Engine PRIVATE EnTT::EnTT yaml-cpp glfw glm::glm)