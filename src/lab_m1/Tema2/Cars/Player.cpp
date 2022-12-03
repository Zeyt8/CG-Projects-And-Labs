#include "Player.h"
#include "../Camera.h"
#include "../Track.h"

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

    // Movement
    glm::vec3 moveDir = glm::vec3(scene->GetCamera()->forward.x, 0, scene->GetCamera()->forward.z);
    moveDir = glm::normalize(moveDir);
    glm::vec3 newPos = position;
    newPos += moveDir * speed * deltaTime;
	if (!scene->GetWindow()->KeyHold(GLFW_KEY_W) && !scene->GetWindow()->KeyHold(GLFW_KEY_S))
	{
        if (speed > 0)
        {
			speed -= deltaTime * 10;
        }
        else if (speed < 0)
        {
            speed += deltaTime * 10;
        }
	}

    // Collision detection
    bool canMove = false;
    const std::vector<VertexFormat> poss = Track::Instance->meshes["track"]->vertices;
    const std::vector<unsigned int> inds = Track::Instance->meshes["track"]->indices;
    for (int i = 0; i < inds.size() - 2; i += 3)
    {
        if (IsInTriangle(poss[inds[i]].position, poss[inds[i + 1]].position, poss[inds[i + 2]].position, newPos))
        {
            canMove = true;
            break;
        }
    }
    for (const Car* car : scene->enemies)
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
}

void Player::OnInputUpdate(float deltaTime, int mods)
{
	Car::OnInputUpdate(deltaTime, mods);
    
    // Movement Input
    if (scene->GetWindow()->KeyHold(GLFW_KEY_W))
    {
        speed += deltaTime * 2;
        if (speed > maxSpeed)
        {
            speed = maxSpeed;
        }
    }
    else if (scene->GetWindow()->KeyHold(GLFW_KEY_S))
    {
		speed -= deltaTime * 2;
        if (speed < -maxSpeed)
        {
            speed = -maxSpeed;
        }
    }

    // Rotation
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

bool Player::IsInTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 newPos) const
{
    const glm::vec3 v0 = C - A;
    const glm::vec3 v1 = B - A;
	const glm::vec3 v2 = newPos - A;

	const float dot00 = glm::dot(v0, v0);
	const float dot01 = glm::dot(v0, v1);
	const float dot02 = glm::dot(v0, v2);
	const float dot11 = glm::dot(v1, v1);
	const float dot12 = glm::dot(v1, v2);

	const float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	const float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	const float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	return (u >= 0) && (v >= 0) && (u + v < 1);
}
