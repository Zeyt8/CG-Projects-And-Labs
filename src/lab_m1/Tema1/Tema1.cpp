#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/GameObject.h"
#include "lab_m1/Tema1/Duck/Duck.h"
#include "lab_m1/Tema1/Player/Player.h"
#include <glm/gtc/random.hpp>

using namespace m1;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution() / 50;
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 100);
	camera->SetPosition(glm::vec3(0, 0, 10));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(true);

	player = new Player(this);
	gameObjects.push_back(player);

	for (GameObject* go : gameObjects)
	{
		go->Awake();
	}
	for (GameObject* go : gameObjects)
	{
		go->Start();
	}
}

void Tema1::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	SimpleScene::RenderMesh(mesh, shader, modelMatrix);
}

void Tema1::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	for (GameObject* go : gameObjects)
	{
		go->Update(deltaTimeSeconds);
	}

	timeSinceDuck += deltaTimeSeconds;
	if (!isDuckInScene)
	{
		duck = new Duck(this, glm::linearRand(1, 2));
		gameObjects.push_back(duck);
		duck->Awake();
		duck->Start();
		isDuckInScene = true;
		player->bullets = 3;
		timeSinceDuck = 0;
	}
	else
	{
		if (timeSinceDuck >= 5)
		{
			duck->Escape();
			delete[] duck;
			player->health--;
		}
	}
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (button == 0)
	{
		Shoot(mouseX, mouseY);
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}

void m1::Tema1::Shoot(int mouseX, int mouseY)
{
}
