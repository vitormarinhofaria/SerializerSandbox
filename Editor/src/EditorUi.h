#pragma once
#include "Scene.h"
#include <GLFW/glfw3.h>

#include "GameProject.h"
#include "ScenePlayer.h"
namespace Editor
{
	class EditorUi
	{
	public:
		EditorUi(void* window, GameProject* gameProject);
		~EditorUi();

		void Draw(Scene& scene, ScenePlayer& scenePlayer);

	private:
		void DrawMainMenuBar(Scene& scene);
		void DrawEntitiesWindow(Scene& scene, ScenePlayer& scenePlayer);
		void DrawEntitiesSpector(Scene& scene, ScenePlayer& scenePlayer);
		void DrawGameView(Scene& scene, ScenePlayer& scenePlayer);

		void PrepNewFrame();
		void DrawFrame();

		void* m_window = nullptr;
		bool m_entitiesWindowShow = true;
		bool m_entitiesSpectorShow = true;
		Entity m_selectedEntity;

		bool m_showDemoWindow = false;

		GameProject* m_gameProject = nullptr;

	};
}