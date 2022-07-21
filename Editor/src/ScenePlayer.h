#pragma once

#include "Scene.h"
#include "SceneSerializer.h"
#include "Components.h"
#include "Renderer.h"
#include <string>

static const char* vertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"uniform mat4 mvp;\n"
"out vec3 Color;\n"
"void main()\n"
"{\n"
"   gl_Position = mvp * vec4(aPos, 1.0);\n"
"   Color = aColor;\n"
"}\0";
static const char* fragmentShaderSource = "#version 450 core\n"
"in vec3 Color;\n"
"out vec4 out_FragColor;\n"
"void main()\n"
"{\n"
"   out_FragColor = vec4(Color, 1.0f);\n"
"}\n\0";

#include "EditorCamera.h"
namespace Editor
{
	enum SceneState
	{
		PLAYING,
		PAUSED,
		STOPPED,
	};
	class ScenePlayer
	{
	public:
		ScenePlayer()
		{
			m_State = STOPPED;
		}
		void Play(Scene& scene)
		{

			if (m_State != PAUSED)
			{
				std::string sceneString = SceneToString(scene);
				m_Scene = SceneFromString(sceneString);

				auto ent = m_Scene.CreateCameraEntity(true);
				auto screenSize = Engine::Renderer::Get()->Resolution();
				auto* cameraComponent = ent.GetComponent<CameraComponent>();
				cameraComponent->OnScreenResize(screenSize.x, screenSize.y);

				ScriptComponent camScript;
				camScript.Bind<EditorCamera>();
				ent.AddComponent<ScriptComponent>(camScript);
				m_Scene.BeginPlay();
				{
					float vertices[] = {
						-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.5f,
						 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f,
						 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
						 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
						-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
						-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.5f,

						-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
						 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
						 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
						 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
						-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.5f,
						-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,

						-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
						-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
						-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
						-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
						-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
						-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,

						 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
						 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
						 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
						 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
						 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
						 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,

						-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
						 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
						 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
						 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
						-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
						-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.5f,

						-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f,
						 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,
						 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
						 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
						-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
						-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.5f
					};
					uint32_t indices[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35 };
					MeshComponent mesh{};
					mesh.m_Shader = Engine::Shader::Create(vertexShaderSource, fragmentShaderSource);
					mesh.m_VertexBuffer = Engine::VertexBuffer::Create(vertices, 6, sizeof(vertices), indices, sizeof(indices));
					auto ents = m_Scene.GetAllEntitites();
					auto& entc = ents[1];
					entc.AddComponent(mesh);
				}
			}
			m_State = PLAYING;
		}
		void Pause()
		{
			if (m_State == PLAYING)
				m_State = PAUSED;
		}
		void Resume()
		{
			if (m_State == PAUSED)
				m_State = PLAYING;
		}
		void Stop()
		{
			if (m_State != STOPPED) {
				m_State = STOPPED;
				m_Scene.Destroy();
			}
		}
		void Tick(float delta)
		{
			if (m_State == PLAYING)
				m_Scene.Tick(delta);
		}
		SceneState m_State;
		Scene m_Scene;

	private:
	};
}
