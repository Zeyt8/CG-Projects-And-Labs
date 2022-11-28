#include "Player.h"
#include "../Camera.h"

using namespace p2;

Player::Player(Tema2* scene) : Car(scene)
{
}

Player::~Player() = default;

void Player::Awake()
{
	Car::Awake();
}

void Player::Start()
{
	Car::Start();
	SetPosition(glm::vec3(2.26f, 0, -1.66f) * 15.0f);
	SetRotation(glm::vec3(0, RADIANS(65), 0));
	SetScale(glm::vec3(1, 1, 2));
}

void Player::Update(float deltaTime)
{
	Car::Update(deltaTime);
}

void Player::OnInputUpdate(float deltaTime, int mods)
{
	Car::OnInputUpdate(deltaTime, mods);
    glm::vec3 moveDir = glm::vec3(scene->GetCamera()->forward.x, 0, scene->GetCamera()->forward.z);
    moveDir = glm::normalize(moveDir);
    if (scene->GetWindow()->KeyHold(GLFW_KEY_W))
    {
        SetPosition(position + moveDir * speed * deltaTime);
    }

    if (scene->GetWindow()->KeyHold(GLFW_KEY_A))
    {
        SetRotation(rotation + glm::vec3(0, 1, 0) * deltaTime);
    }

    if (scene->GetWindow()->KeyHold(GLFW_KEY_S))
    {
        SetPosition(position - moveDir * speed * deltaTime);
    }

    if (scene->GetWindow()->KeyHold(GLFW_KEY_D))
    {
        SetRotation(rotation - glm::vec3(0, 1, 0) * deltaTime);
    }
}

void Player::Render()
{
	Car::Render();
}
