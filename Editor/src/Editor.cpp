//module;

#include <Windows.h>
#include <string>
#include <thread>
#include <iostream>
#include <filesystem>

#include "Scene.h"
#include "Components.h"
#include "SceneSerializer.h"
#include "Renderer.h"

#include "imgui.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"


#include "EditorUi.h"
#include "GameProject.h"
#include "ScenePlayer.h"

std::filesystem::path EditorWorkDir;
Editor::GameProject* CurrentProject = nullptr;

int main(int argc, char** argv)
{
	EditorWorkDir = std::filesystem::current_path();

	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Janela", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	gladLoadGL(glfwGetProcAddress);

	Engine::Renderer::Init(Engine::RendererApi::OpenGL, window);

	Editor::EditorUi editorUi = Editor::EditorUi(window, CurrentProject);

	Scene scene;
	Editor::ScenePlayer scenePlayer;

	auto lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		editorUi.Draw(scene, scenePlayer);
		
		auto current = glfwGetTime();
		auto delta = current - lastTime;
		
		scenePlayer.Tick(delta);
		
		lastTime = glfwGetTime();

		glfwSwapBuffers(window);
	}

	SceneToTextFile(scene, "assets/scenes/untitled.yaml");

	if (CurrentProject)
		delete CurrentProject;

	std::filesystem::current_path(EditorWorkDir);

	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();

}