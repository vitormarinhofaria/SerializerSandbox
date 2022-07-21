#include "Components.h"

CameraComponent::CameraComponent(float width, float height, float fov, float nearPlane, float farPlane)
{
	m_Foward = { 0.0, 0.0, -1.0 };
	m_Right = { 1.0f, 0.0f, 0.0f };
	m_Up = {0.0f, 1.0f, 0.0f};
	m_FieldOfView = fov;
	m_NearPlane = nearPlane;
	m_FarPlane = farPlane;

	m_Projection = glm::perspectiveFov(glm::radians(fov), width, height, nearPlane, farPlane);
	UpdateRollPitchYaw(0, 0, 0);
}

glm::mat4 CameraComponent::GetView(glm::vec3 position)
{
	return glm::lookAt(position, m_Foward + position, m_Up);
}

void CameraComponent::OnScreenResize(float width, float height, float fov, float nearPlane, float farPlane)
{
	m_Projection = glm::perspectiveFov(glm::radians(fov), width, height, nearPlane, farPlane);
	m_FieldOfView = fov;
	m_FarPlane = farPlane;
	m_NearPlane = nearPlane;
	UpdateRollPitchYaw(0, 0, 0);
}

PUBLIC void CameraComponent::UpdateRollPitchYaw(float roll, float pitch, float yaw)
{
	m_Roll = roll;
	m_Pitch = pitch;
	m_Yaw = yaw;
	//glm::rotate()
	glm::vec3 front{};
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_Foward = glm::normalize(front);
	m_Right = glm::normalize(glm::cross(m_Right, m_Foward));
	//auto rotation = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);	
	
	//m_Foward = XMVector3Normalize(XMVector3Rotate(XMVectorSet(0.0f, 0.0f, -0.1f, 0.0f), rotation));
	//m_Right = XMVector3Normalize(XMVector3Cross(m_Up, m_Foward));
	
}
