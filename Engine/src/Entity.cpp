#include "Entity.h"

Entity::Entity(entt::registry& world, entt::entity ent)
{
	m_Entity = ent;
	m_World = &world;
}

bool Entity::operator == (Entity& rhs)
{
	return m_Entity == rhs.m_Entity;
}