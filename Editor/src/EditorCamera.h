#pragma once

#include "Common.h"
#include "Components.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Editor
{
	struct EditorCamera : public Behavior
	{
		CameraComponent* camera;
		TransformComponent* transform;
		GLFWwindow* window;
		float moveSpeed = 10.0f;
		void OnCreate() override
		{
			window = glfwGetCurrentContext();
			camera = this->m_Entity.GetComponent<CameraComponent>();
			transform = this->m_Entity.GetComponent<TransformComponent>();
		}
		void OnTick(float deltaTime) override
		{
			float tMoveSpeed = moveSpeed * deltaTime;
			//auto actualMoveSpeed = XMVector3Normalize(XMVectorSet(tMoveSpeed, tMoveSpeed, tMoveSpeed, tMoveSpeed));
			//auto position = XMVectorSet(transform->Position.x, transform->Position.y, transform->Position.z, 0);

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				std::cout << "CAMERA W\n";
				//position = XMVectorAdd(position, XMVectorMultiply(camera->m_Foward * -1.0f, actualMoveSpeed));
				transform->Position += -camera->m_Foward * tMoveSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				transform->Position += camera->m_Foward * tMoveSpeed;
				//position = XMVectorAdd(position, XMVectorMultiply(camera->m_Foward, actualMoveSpeed));
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				//auto v = XMVector3Normalize(XMVector3Cross(camera->m_Up, camera->m_Foward));
				glm::vec3 v = glm::normalize(glm::cross(camera->m_Up, camera->m_Foward));
				transform->Position += -v * tMoveSpeed;
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				glm::vec3 v = glm::normalize(glm::cross(camera->m_Up, camera->m_Foward));
				transform->Position += v * tMoveSpeed;
			}
		}
	};

}
