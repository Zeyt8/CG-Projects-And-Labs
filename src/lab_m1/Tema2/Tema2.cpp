#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/GameObject.h"
#include "lab_m1/Tema2/Track.h"
#include "lab_m1/Tema2/Camera.h"

using namespace p2;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	glm::ivec2 resolution = window->GetResolution() / 50;
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 10));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(true);

	Track* track = new Track(this);
	gameObjects.push_back(track);

	for (GameObject* go : gameObjects)
	{
		go->Awake();
	}
	for (GameObject* go : gameObjects)
	{
		go->Start();
	}
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	SimpleScene::RenderMesh(mesh, shader, modelMatrix);
}

void Tema2::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.23f, 0.71f, 0.87f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Tema2::Update(float deltaTimeSeconds)
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
}

void Tema2::FrameEnd()
{
	DrawCoordinateSystem();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema2::OnKeyPress(int key, int mods)
{
}

void Tema2::OnKeyRelease(int key, int mods)
{
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
