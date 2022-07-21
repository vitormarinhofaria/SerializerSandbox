#pragma once
#include "Common.h"
#include "Entity.h"

#include "Buffer.h"
#include "Shader.h"

#include <string>
#include <map>
#include <functional>
#include <iostream>
#include <random>

#include <glm/ext.hpp>

struct CameraComponent
{
	PUBLIC CameraComponent(float width = 800, float height = 600, float fov = 90.0f, float nearPlane = 0.01f, float farPlane = 100.0f);
	PUBLIC glm::mat4 GetView(glm::vec3 position);

	PUBLIC void OnScreenResize(float width = 800, float height = 600, float fov = 90.0f, float nearPlane = 0.01f, float farPlane = 100.0f);
	PUBLIC void UpdateRollPitchYaw(float roll, float pitch, float yaw);

	glm::vec3 m_Position = { 0.0f, 0.0f, 25.0f };
	glm::mat4 m_Projection = glm::mat4();

	/// <summary>
	/// FOV (in degrees)
	/// </summary>
	float m_FieldOfView;
	float m_FarPlane;
	float m_NearPlane;

	float m_Yaw = 0.0f;
	float m_Pitch = 0.0f;
	float m_Roll = 0.0f;

	glm::vec3 m_Up;
	glm::vec3 m_Foward;
	glm::vec3 m_Right;
};

struct UUIDComponent
{
	uint64_t beg;
	uint64_t end;

	UUIDComponent()
	{
		beg = std::rand();
		beg = ((uint64_t)std::rand() << sizeof(int));
		end = std::rand();
		end = ((uint64_t)std::rand() << sizeof(int));
	}
	UUIDComponent(uint64_t id) : beg(id), end(0) {}
};

struct Vec3f
{
	float x;
	float y;
	float z;
};

struct TransformComponent
{
	glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
};

constexpr size_t MAX_TAG_SIZE = 128;
struct TagComponent
{
	PUBLIC TagComponent(const char* name)
	{
		strcpy_s(m_Name, name);
	};
	PUBLIC TagComponent(char* name)
	{
		strcpy_s(m_Name, name);
	};
	PUBLIC const char* Tag()
	{
		return m_Name;
	}
	PUBLIC void Tag(char* name)
	{
		strcpy_s(m_Name, name);
	}
	PUBLIC void Tag(const char* name)
	{
		strcpy_s(m_Name, name);
	}

private:
	char m_Name[MAX_TAG_SIZE];
};

struct Behavior
{
	virtual void OnTick(float deltaTime) {};
	virtual void OnCreate() {};
	virtual void OnDestroy() {};
	Entity m_Entity;
};


template <typename T>
std::string GetTypeName()
{
	std::string name = typeid(T).name();
	auto spos = name.find("struct");
	if (spos != std::string::npos)
	{
		name.replace(spos, strlen("struct") + 1, "");
	}
	else
	{
		spos = name.find("class");
		if (spos != std::string::npos)
		{
			name.replace(spos, strlen("class") + 1, "");
		}
	}
	return name;
}

struct ScriptComponent
{
	Behavior* m_Instance = nullptr;

	Behavior* (*InstantiateScript)();
	void (*DestroyScript)(ScriptComponent*);

	std::string m_BehaviorName;

	template <typename T>
	void Bind()
	{
		InstantiateScript = []()
		{
			return static_cast<Behavior*>(new T());
		};
		DestroyScript = [](ScriptComponent* sc)
		{delete sc->m_Instance; sc->m_Instance = nullptr; };

		m_BehaviorName = GetTypeName<T>();
	}
};

struct MeshComponent
{
	Engine::VertexBuffer* m_VertexBuffer;
	Engine::IndexBuffer* m_IndexBuffer;
	Engine::Shader* m_Shader;
};


template <typename T>
void register_class(std::map<std::string, ScriptComponent>* map)
{
	std::string name = GetTypeName<T>();
	std::cout << "Registered Class: " << name << '\n';
	ScriptComponent script;
	script.Bind<T>();
	(*map)[name] = script;
}
