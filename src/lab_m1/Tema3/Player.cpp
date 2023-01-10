#include "Player.h"

using namespace p3;

Player::Player(Tema3* scene) : GameObject(scene)
{
    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(scene->SourcePrimitiveDir, "box.obj");
    Meshes[mesh->GetMeshID()] = mesh;

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(scene->SourceTextureDir, "Player.jpg").c_str(), GL_REPEAT);
        Scene->Textures["player"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(scene->SourceTextureDir, "Ski.jpg").c_str(), GL_REPEAT);
        Scene->Textures["ski"] = texture;
    }

	SetRotation(glm::vec3(RADIANS(30), 0, 0));
    SetScale(glm::vec3(0.8f, 1, 0.8f));
}

Player::~Player() = default;

void Player::Update(float deltaTime)
{
    SetPosition(Position + Forward * _speed * deltaTime);
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