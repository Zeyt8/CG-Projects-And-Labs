#include "Player.h"
#include "Gift.h"
#include "Rock.h"
#include "Lamp.h"
#include "Tree.h"
#include <iostream>

using namespace p3;

Player::Player(Tema3* scene) : GameObject(scene)
{
    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(scene->SourcePrimitiveDir, "box.obj");
    Meshes[mesh->GetMeshID()] = mesh;

	SetRotation(glm::vec3(RADIANS(30), 0, 0));
    SetScale(glm::vec3(0.8f, 1, 0.8f));
    ColliderRadius = 0.5f;
}

void Player::Update(float deltaTime)
{
    if (_canMove)
    {
    	SetPosition(Position + Forward * _speed * deltaTime);
    }
}

void Player::Render()
{
	glm::mat4 modelMatrix = TranslateMatrix(ModelMatrix, glm::vec3(0, 0.6f, 0));
    Scene->RenderMesh(Meshes["box"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["player"]);
	modelMatrix = TranslateMatrix(ModelMatrix, glm::vec3(0.4f, 0.05f, 0));
    modelMatrix = ScaleMatrix(modelMatrix, glm::vec3(0.2f, 0.1f, 2));
	Scene->RenderMesh(Meshes["box"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["ski"]);
    modelMatrix = TranslateMatrix(ModelMatrix, glm::vec3(-0.4f, 0.05f, 0));
    modelMatrix = ScaleMatrix(modelMatrix, glm::vec3(0.2f, 0.1f, 2));
    Scene->RenderMesh(Meshes["box"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["ski"]);
}

void Player::OnInputUpdate(float deltaTime, int mods)
{
	glm::vec2 mousePos = Scene->GetWindow()->GetCursorPosition();
	glm::vec2 res = Scene->GetWindow()->GetResolution();
	mousePos.y = res.y - mousePos.y;
	glm::vec2 dir = res / 2.0f - mousePos;
    dir = glm::normalize(dir);
    
    float angle = glm::acos(glm::dot(glm::vec2(0, 1), dir));
    angle *= dir.x > 0 ? -1 : 1;
    angle = glm::max(RADIANS(-45), glm::min(RADIANS(45), angle));
    SetRotation(glm::vec3(Rotation.x, angle, Rotation.z));
}

void Player::OnCollisionEnter(GameObject* other)
{
	if (dynamic_cast<Gift*>(other))
	{
        Score++;
        other->Destroy();
	}
    if (dynamic_cast<Rock*>(other) || dynamic_cast<Tree*>(other) || dynamic_cast<Lamp*>(other))
    {
		_canMove = false;
        std::cout << "Score: " << Score << std::endl;
    }
}
