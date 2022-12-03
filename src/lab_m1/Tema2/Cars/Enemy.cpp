#include "Enemy.h"
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

using namespace p2;

Enemy::Enemy(Tema2* scene, glm::vec3 color) : Car(scene, color)
{
}

Enemy::~Enemy() = default;

void Enemy::Awake()
{
	Car::Awake();
}

void Enemy::Start()
{
	Car::Start();
	SetPosition(path[0]);
	nextNode = 1;
	speed = 6;
}

void Enemy::Update(float deltaTime)
{
	//Car::Update(deltaTime);
	SetPosition(position + glm::normalize(path[nextNode] - position) * speed * deltaTime);
	if (glm::distance(path[nextNode], position) < 0.1f)
	{
		nextNode++;
		if (nextNode >= path.size())
		{
			nextNode = 0;
		}
		SetRotation(glm::vec3(0, glm::orientedAngle(glm::vec3(0, 0, -1), glm::normalize(path[nextNode] - position), glm::vec3(0, 1, 0)), 0));
	}
}

void Enemy::OnInputUpdate(float deltaTime, int mods)
{
	Car::OnInputUpdate(deltaTime, mods);
}

void Enemy::Render()
{
	Car::Render();
}
