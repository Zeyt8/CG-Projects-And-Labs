#include "Minimap.h"
#include "Camera.h"

using namespace p2;

Minimap::Minimap(Tema2* scene, Car* car) : GameObject(scene)
{
	camera = new Camera();
	camera->followTarget = car;
	camera->distanceToTarget = 20;
	camera->RotateThirdPerson_OX(RADIANS(-90));
}

Minimap::~Minimap() = default;

void Minimap::Awake()
{
	scene->AddObject(camera);
}

void Minimap::Start()
{
}

void Minimap::Update(float deltaTime)
{
}

void Minimap::LateUpdate(float deltaTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(50, 50, scene->GetWindow()->GetResolution().x / 4.0f, scene->GetWindow()->GetResolution().y / 4.0f);

	glm::mat4 oldProj = scene->projectionMatrix;
	const float size = 20.0f;
	scene->projectionMatrix = glm::ortho(-size * scene->GetWindow()->props.aspectRatio / 2, size * scene->GetWindow()->props.aspectRatio / 2, -size / 2, size / 2, 0.1f, 100.0f);

	Camera* oldCamera = scene->GetCamera();
	scene->SetCamera(camera);

	for (int i = 0; i < scene->gameObjects.size(); i++)
	{
		scene->gameObjects[i]->Render();
	}

	glViewport(0, 0, scene->GetWindow()->GetResolution().x, scene->GetWindow()->GetResolution().y);
	scene->SetCamera(oldCamera);
	scene->projectionMatrix = oldProj;
}

void Minimap::Render()
{
}
