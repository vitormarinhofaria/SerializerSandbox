#pragma once

#include "Common.h"
#include <entt/entt.hpp>

struct Entity
{
public:
	template <typename T>
	void AddComponent(T value)
	{
		m_World->emplace<T>(m_Entity, value);
	};
	template <typename T>
	void AddComponent()
	{
		m_World->emplace<T>(m_Entity);
	};
	template <typename T>
	T* GetComponent()
	{
		return m_World->try_get<T>(m_Entity);
	}
	Entity() {
	};
	friend class Scene;

	PUBLIC bool operator == (Entity& rhs);

	PUBLIC Entity(entt::registry& world, entt::entity entity);
	entt::entity m_Entity;
private:
	entt::registry* m_World;
};
