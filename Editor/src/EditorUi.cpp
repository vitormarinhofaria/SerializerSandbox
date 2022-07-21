#include <format>
#include <filesystem>

#include "glad/gl.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene.h"
#include "Components.h"
#include "SceneSerializer.h"
#include "Renderer.h"

#include "EditorUi.h"
#include "GameProject.h"
#include "ScenePlayer.h"
#include "EditorStyles.h"
#include "EditorCamera.h"

namespace Editor
{
	static Entity s_EditorCamera;
	EditorUi::EditorUi(void* window, GameProject* gameProject)
	{
		m_gameProject = gameProject;
		m_window = window;
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(m_window), true);
		ImGui_ImplOpenGL3_Init();

		EditorStyles::AltDarkTheme();
	}
	EditorUi::~EditorUi()
	{}

	void EditorUi::Draw(Scene& scene, ScenePlayer& scenePlayer)
	{
		Scene* p_Scene = (scenePlayer.m_State != STOPPED) ? &scenePlayer.m_Scene : &scene;

		PrepNewFrame();

		DrawMainMenuBar(*p_Scene);

		if (m_gameProject != nullptr) {
			if (m_entitiesWindowShow)
				DrawEntitiesWindow(*p_Scene, scenePlayer);

			if (m_entitiesSpectorShow)
				DrawEntitiesSpector(*p_Scene, scenePlayer);
		}

		if (m_showDemoWindow)
			ImGui::ShowDemoWindow(&m_showDemoWindow);

		DrawGameView(*p_Scene, scenePlayer);

		DrawFrame();
	}

	void EditorUi::DrawMainMenuBar(Scene& scene)
	{
		static bool popNew = false;
		static bool popOpen = false;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Project")) popNew = true;
				if (ImGui::MenuItem("Open")) popOpen = true;
				if (ImGui::MenuItem("Exit")) glfwSetWindowShouldClose(glfwGetCurrentContext(), true);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Build"))
			{
				if (ImGui::MenuItem("Compile Game Code"))
				{
					m_gameProject->CompileCode();
					m_gameProject->ReloadCode();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Show ImGui Demo Window"))
					m_showDemoWindow = true;
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		{
			if (popNew) { ImGui::OpenPopup("NewSettings"); popNew = false; };
			if (popOpen) { ImGui::OpenPopup("OpenSettings"); popOpen = false; }

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("NewSettings", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				static char nameBuffer[512] = { 0 };
				ImGui::InputText("Project Name", nameBuffer, sizeof(nameBuffer));

				static char dirBuffer[512] = { 0 };
				ImGui::InputText("Project Directory", dirBuffer, sizeof(dirBuffer));

				static char templateBuffer[512] = { "C:/src/cpp/GameProjectTemplate\0" };
				ImGui::InputText("Template Directory", templateBuffer, sizeof(templateBuffer));
				if (ImGui::Button("Cancel"))
					ImGui::CloseCurrentPopup();

				ImGui::SameLine();
				if (ImGui::Button("Create"))
				{
					m_gameProject = Editor::GameProject::New(nameBuffer, dirBuffer, templateBuffer);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopupModal("OpenSettings"))
			{
				static char dirBuffer[512] = "C:/src/cpp/GameProjectTeste";
				ImGui::InputText("Project Directory", dirBuffer, sizeof(dirBuffer));

				if (ImGui::Button("Cancel"))
					ImGui::CloseCurrentPopup();

				ImGui::SameLine();
				if (ImGui::Button("Open"))
				{
					if (strlen(dirBuffer) > 0) {
						m_gameProject = Editor::GameProject::Open(dirBuffer);
						register_class<EditorCamera>(&m_gameProject->m_RegisteredScripts);
						m_gameProject->m_RegisteredScriptNames.push_back("Editor::EditorCamera");

						if (std::filesystem::exists("assets/scenes/untitled.yaml")) {
							scene = SceneFromTextFile("assets/scenes/untitled.yaml");
							//s_EditorCamera = scene.CreateCameraEntity(true);
							//auto camera = s_EditorCamera.GetComponent<CameraComponent>();
							//s_EditorCamera.GetComponent<TagComponent>()->Tag("Editor Camera");
							
							//s_EditorCamera.AddComponent<ScriptComponent>(m_gameProject->m_RegisteredScripts["Editor::EditorCamera"]);
						}
						glfwSetWindowTitle(glfwGetCurrentContext(), std::format("{} - {}", m_gameProject->m_ProjectName, scene.m_Name).c_str());
					}
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			//ImGui::End();
		}
	}
	void EditorUi::DrawEntitiesWindow(Scene& scene, ScenePlayer& scenePlayer)
	{
		ImGui::Begin("Entities");

		if (ImGui::BeginPopupContextWindow()) // <-- use last item id as popup id
		{
			static int selectedItem = 0;
			if (selectedItem > m_gameProject->m_RegisteredScriptNames.size())
				selectedItem = 0;

			ImGui::Text("New Entity");
			ImGui::Combo("ScriptComponent", &selectedItem, m_gameProject->m_RegisteredScriptNames.data(), m_gameProject->m_RegisteredScriptNames.size());
			if (ImGui::Button("New"))
			{
				auto entity = scene.CreateEntity();
				entity.AddComponent<ScriptComponent>(m_gameProject->m_RegisteredScripts[m_gameProject->m_RegisteredScriptNames[selectedItem]]);
				auto* script = entity.GetComponent<ScriptComponent>();
				script->m_Instance = script->InstantiateScript();
				script->m_Instance->m_Entity = entity;
				script->m_Instance->OnCreate();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Delete"))
			{
				scene.m_World.destroy(m_selectedEntity.m_Entity);
			}
			ImGui::EndPopup();
		}

		auto entities = scene.GetAllEntitites();
		for (auto& entity : entities)
		{
			TagComponent* tag = entity.GetComponent<TagComponent>();
			UUIDComponent* uuid = entity.GetComponent<UUIDComponent>();
			if (ImGui::Selectable(std::format("{} :{}", tag->Tag(), uuid->beg).c_str(), m_selectedEntity == entity))
			{
				m_selectedEntity = entity;
			}
			ImGui::Separator();
		}

		ImGui::End();
	}

	void DrawTransformComponentInspector(TransformComponent* transform)
	{
		ImGui::Text("TransformComponent");
		ImGui::DragFloat3("Position", (float*)&transform->Position, 0.1f, -10000.0f, 10000.0f, "%.2f", 1.0f);
		ImGui::DragFloat3("Rotation", (float*)&transform->Rotation, 0.1f, -10000.0f, 10000.0f, "%.2f", 1.0f);
		ImGui::DragFloat3("Scale", (float*)&transform->Scale, 0.1f, 0.0f, 10000.0f, "%.2f", 1.0f);
	}

	void DrawTagComponnentInspector(TagComponent* tag)
	{
		ImGui::Text("TagComponent");
		char buffer[MAX_TAG_SIZE] = { 0 };
		strcpy_s(buffer, tag->Tag());

		if (ImGui::InputText("TagComponent", buffer, 128)) {
			tag->Tag(buffer);
			//buffer[0] = '\0';
		}
	}

	const char* vertexShaderSource = "#version 450 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 mvp;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = mvp * vec4(aPos, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 450 core\n"
		"out vec4 out_FragColor;\n"
		"void main()\n"
		"{\n"
		"   out_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	void DrawAddComponentContextMenu(Scene& scene, Entity& entity)
	{
		if (ImGui::BeginPopupContextWindow()) // <-- use last item id as popup id
		{
			if (ImGui::Button("Cube Mesh"))
			{
				float vertices[] = {
					0.5f, 0.5f, 0.5f,
					0.5f, -0.5f, 0.5f,
					-0.5f, -0.5f, 0.5f,
					-0.5f, 0.5f, 0.5f,

					0.5f, 0.5f, -0.5f,
					0.5f, -0.5f, -0.5f,
					-0.5f, -0.5f, -0.5f,
					-0.5f, 0.5f, -0.5f
				};
				uint32_t indices[] = { 0,1,3,1,2,3 };
				MeshComponent mesh{};
				mesh.m_Shader = Engine::Shader::Create(vertexShaderSource, fragmentShaderSource);
				mesh.m_VertexBuffer = Engine::VertexBuffer::Create(vertices, 3, sizeof(vertices), indices, sizeof(indices));

				entity.AddComponent(mesh);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void DrawMeshComponentInspector(MeshComponent* mesh)
	{
		ImGui::Text("MeshComponent");
		ImGui::Text("Vertex Count %ud", mesh->m_VertexBuffer->VerticesCount());
		ImGui::Text("Indices Count %ud", mesh->m_VertexBuffer->IndicesCount());
	}

	void EditorUi::DrawEntitiesSpector(Scene& scene, ScenePlayer& scenePlayer)
	{
		if (!scene.ValidEntity(m_selectedEntity)) {
			ImGui::Begin("Inspector");
			ImGui::End();
			return;
		}

		ImGui::Begin("Inspector");
		ImGui::Text("Entity ID: %d", (int)(*(entt::entity*)&m_selectedEntity));
		ImGui::Separator();

		auto* transformComponent = m_selectedEntity.GetComponent<TransformComponent>();
		DrawTransformComponentInspector(transformComponent);
		ImGui::Separator();

		auto* tagComponent = m_selectedEntity.GetComponent<TagComponent>();
		DrawTagComponnentInspector(tagComponent);
		ImGui::Separator();

		MeshComponent* meshComponent = m_selectedEntity.GetComponent<MeshComponent>();
		if (meshComponent != nullptr)
		{
			DrawMeshComponentInspector(meshComponent);
			ImGui::Separator();
		}

		DrawAddComponentContextMenu(scene, m_selectedEntity);

		ImGui::End();
	}

	void EditorUi::DrawGameView(Scene& scene, ScenePlayer& scenePlayer)
	{

		{
			ImGui::Begin("Controls");
			bool playing = scenePlayer.m_State != STOPPED;
			if (playing)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}
			if (ImGui::Button("Play"))
			{
				scenePlayer.Play(scene);
			}
			if (playing)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar(1);
			}

			ImGui::SameLine();
			if (scenePlayer.m_State == PAUSED)
			{
				if (ImGui::Button("Resume"))
					scenePlayer.Resume();

			}
			else {
				if (ImGui::Button("Pause"))
					scenePlayer.Pause();
			}

			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				scenePlayer.Stop();
			}
			ImGui::End();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
		ImGui::Begin("Viewport");

		auto* renderer = Engine::Renderer::Get();

		static ImVec2 prevWindowSize = ImGui::GetContentRegionAvail();
		ImVec2 newSize = ImGui::GetContentRegionAvail();
		if (prevWindowSize.x != newSize.x || prevWindowSize.y != newSize.y)
		{
			auto windowSize = ImGui::GetWindowSize();
			renderer->Resolution({ (int)newSize.x, (int)newSize.y });
			prevWindowSize = newSize;

			//auto camera = scene.GetMainCamera().GetComponent<CameraComponent>();
		}

		renderer->Render(scene);
		ImGui::Image(renderer->Present(), { newSize.x, newSize.y }, { 0.0f,1.0f }, { 1,0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorUi::PrepNewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();
	}
	void EditorUi::DrawFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			//GLFWwindow* contextBackup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_window));
		}
	}
}