#include "Player.h"
#include "../Camera.h"

using namespace p2;

Player::Player(Tema2* scene, glm::vec3 color) : Car(scene, color)
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

    glm::vec3 newPos = position;
    if (scene->GetWindow()->KeyHold(GLFW_KEY_W))
    {
        newPos += moveDir * speed * deltaTime;
    }
    if (scene->GetWindow()->KeyHold(GLFW_KEY_S))
    {
        newPos -= moveDir * speed * deltaTime;
    }

    bool canMove = true;
	for (Car* car : scene->enemies)
	{
		if (glm::distance(newPos, car->position) < 1.5f)
		{
			canMove = false;
			break;
		}
	}
    if (canMove)
    {
        SetPosition(newPos);
    }

    if (scene->GetWindow()->KeyHold(GLFW_KEY_A))
    {
        SetRotation(rotation + glm::vec3(0, 1, 0) * deltaTime);
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
