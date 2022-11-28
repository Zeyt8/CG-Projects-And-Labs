#include "Enemy.h"

using namespace p2;

Enemy::Enemy(Tema2* scene) : Car(scene)
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
}

void Enemy::Update(float deltaTime)
{
	Car::Update(deltaTime);
}

void Enemy::OnInputUpdate(float deltaTime, int mods)
{
	Car::OnInputUpdate(deltaTime, mods);
}

void Enemy::Render()
{
	Car::Render();
}
