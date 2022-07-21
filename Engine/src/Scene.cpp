#include "Scene.h"
#include "Components.h"

Scene::Scene(): m_Name("Untitled")
{
}

Scene::Scene(std::string_view sceneName) : m_Name(sceneName)
{
}

void Scene::BeginPlay()
{
	auto view = m_World.view<ScriptComponent, TagComponent>();
	for (auto [entity, component, tag] : view.each())
	{
		auto tacg = tag.Tag();
		component.m_Instance = component.InstantiateScript();
		component.m_Instance->m_Entity = Entity{ m_World, entity };
		component.m_Instance->OnCreate();
	}
}

void Scene::Destroy()
{
	auto view = m_World.view<ScriptComponent>();
	for (auto [entity, component] : view.each())
	{
		component.m_Instance->OnDestroy();
		component.DestroyScript(&component);
	}
}

void Scene::Tick(float deltaTime)
{
	auto view = m_World.view<ScriptComponent>();
	for (auto [entity, component] : view.each())
	{
		component.m_Instance->OnTick(deltaTime);
	}
}

std::vector<Entity> Scene::GetAllEntitites()
{
	std::vector<Entity> entities;
	auto view = m_World.view<TagComponent>();
	for (auto [entity, tag] : view.each())
	{
		entities.push_back({ m_World, entity });
	}
	return entities;
}

bool Scene::ValidEntity(Entity& entity) 
{
	return m_World.valid(entity.m_Entity);
}

Entity Scene::CreateCameraEntity(bool mainCamera)
{
	auto entity = CreateEntity();
	entity.AddComponent<CameraComponent>();
	entity.GetComponent<TagComponent>()->Tag("Camera");
	
	if (mainCamera) m_CurrentCamera = entity;

	return entity;
}

void Scene::SetMainCamera(Entity& camera)
{
	m_CurrentCamera = camera;
}

Entity Scene::GetMainCamera()
{
	return m_CurrentCamera;
}

Entity Scene::CreateEntity()
{
	entt::entity ent = m_World.create();
	Entity entity = Entity(m_World, ent);
	entity.AddComponent<UUIDComponent>();
	entity.AddComponent<TransformComponent>();
	entity.AddComponent<TagComponent>("Component");
	return entity;
}