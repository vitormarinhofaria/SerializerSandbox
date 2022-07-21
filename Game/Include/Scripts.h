#pragma once

#include <Scene.h>
#include <Components.h>
#include <Windows.h>

struct Camera : public Behavior
{
	float yaw = 0.0f;
	float roll = 0.0f;
	float pitch = 0.0f;

	void OnTick(float deltaTime) override
	{
		float deltaTime2 = deltaTime / 2.0f;
		std::cout << "Camera::OnTick new32\n";
	}
	void OnCreate() override
	{
		std::cout << "Camera::OnCreate yaw: " << yaw << " pith: " << pitch << " roll: " << roll << '\n';
	}
};

struct Player : public Behavior
{
	uint32_t life;
	uint32_t mana;

	void OnTick(float deltaTime) override
	{
		std::cout << "Player::OnTick\n";
	}
	void OnCreate() override
	{
		std::cout << "Player::OnCreate life: " << life << " mana: " << mana << '\n';
	}
};

struct Din : public Behavior
{
	void OnTick(float deltaTime) override
	{
		std::cout << "Din::OnTick()\n";
	}
};

struct Vida : public Behavior
{
	int vida = 0;
	void OnTick(float deltaTime) override
	{
		std::cout << "Minha Vida: " << vida << '\n';
	}

	void OnCreate() override
	{
		vida = GetTickCount64() / 1000;
	}
};