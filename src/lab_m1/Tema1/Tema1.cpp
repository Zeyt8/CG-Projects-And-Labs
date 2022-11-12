#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/GameObject.h"
#include "lab_m1/Tema1/Duck/Duck.h"
#include "lab_m1/Tema1/Player/Player.h"
#include "lab_m1/Tema1/Grass.h"
#include "lab_m1/Tema1/Crosshair.h"
#include "lab_m1/Tema1/Gun.h"
#include "lab_m1/Tema1/Duck/BossDuck.h"
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
	Grass* grass = new Grass(this);
	gameObjects.push_back(grass);
	Crosshair* crosshair = new Crosshair(this);
	gameObjects.push_back(crosshair);
	Gun* gun = new Gun(this);
	gameObjects.push_back(gun);

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
	glClearColor(0.23f, 0.71f, 0.87f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	for (GameObject* go : gameObjects)
	{
		if (go->Destroy)
		{
			gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), go), gameObjects.end());
			delete go;
			continue;
		}
		go->Update(deltaTimeSeconds);
	}

	timeSinceDuck += deltaTimeSeconds;
	if (!isDuckInScene && player->score < 10 && player->health > 0)
	{
		ducks++;
		if (ducks == 6)
		{
			duck = new BossDuck(this);
		}
		else
		{
			duck = new Duck(this, 3 + (ducks / 5) * 2);
		}
		gameObjects.push_back(duck);
		duck->Awake();
		duck->Start();
		isDuckInScene = true;
		player->bullets = 3;
		timeSinceDuck = 0;
	}
	else
	{
		if (timeSinceDuck >= 9)
		{
			duck->Escape();
			timeSinceDuck = 0;
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
	if (button == 1)
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

void Tema1::Shoot(int mouseX, int mouseY)
{
	if (player->bullets <= 0)
	{
		return;
	}
	float newMouseX = (float)mouseX / window->GetResolution().x * GetSceneCamera()->GetProjectionInfo().width;
	float newMouseY = GetSceneCamera()->GetProjectionInfo().height - (float)mouseY / window->GetResolution().y * GetSceneCamera()->GetProjectionInfo().height;
	if (duck->IsInBounds(newMouseX, newMouseY))
	{
		duck->TakeDamage();
	}
	player->bullets--;
	if (player->bullets <= 0)
	{
		duck->Escape();
	}
}
