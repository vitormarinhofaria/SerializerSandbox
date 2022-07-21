#pragma once
#include "Common.h"
#include "Components.h"
#include "Entity.h"

#include <entt/entt.hpp>

#include <map>

struct Scene
{
	PUBLIC Scene();
	PUBLIC Scene(std::string_view sceneName);

	PUBLIC Entity CreateEntity();
	PUBLIC void Tick(float deltaTime);
	
	PUBLIC void BeginPlay();
	PUBLIC void Destroy();
	PUBLIC std::vector<Entity> GetAllEntitites();
	PUBLIC bool ValidEntity(Entity& entity);

	PUBLIC Entity CreateCameraEntity(bool mainCamera = false);
	PUBLIC void SetMainCamera(Entity& camera);
	PUBLIC Entity GetMainCamera();

	entt::registry m_World;
	std::string m_Name;
	Entity m_CurrentCamera;
};