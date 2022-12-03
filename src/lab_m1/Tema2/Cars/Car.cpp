#include "Car.h"

using namespace p2;

Car::Car(Tema2* scene, glm::vec3 color) : GameObject(scene)
{
    const std::vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-0.5f, 0.1f, 1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5f, 0.1f, 1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-0.5f, 1, 1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5f, 1, 1), color, glm::vec3(0.2, 0.8, 0.6)),
		
        VertexFormat(glm::vec3(-0.5f, 0.1f, -1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5f, 0.1f, -1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-0.5f, 1, -1), color, glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5f, 1, -1), color, glm::vec3(0.2, 0.8, 0.6)),
    };

    const std::vector<unsigned int> indices =
    {
        0, 1, 2,
        1, 3, 2,
        2, 3, 7,
        2, 7, 6,
        1, 7, 3,
        1, 5, 7,
        6, 7, 4,
        7, 5, 4,
        0, 4, 1,
        1, 4, 5,
        2, 6, 4,
        0, 2, 4,
    };
	
    CreateMesh("car", vertices, indices);

    for (int i = 0; i < 4; i++)
    {
        Wheel* wheel = new Wheel(scene);
        wheel->parent = this;
        children.push_back(wheel);
		wheels.push_back(wheel);
		scene->AddObject(wheel);
	}
	wheels[0]->SetPosition(glm::vec3(-0.6f, 0.2f, 0.6f));
	wheels[1]->SetPosition(glm::vec3(0.6f, 0.2f, 0.6f));
	wheels[2]->SetPosition(glm::vec3(-0.6f, 0.2f, -0.6f));
	wheels[3]->SetPosition(glm::vec3(0.6f, 0.2f, -0.6f));
}

Car::~Car() = default;

void Car::Awake()
{
}

void Car::Start()
{
    SetRotation(glm::vec3(0, RADIANS(71), 0));
}

void Car::Update(float deltaTime)
{
    for (Wheel* wheel : wheels)
    {
        wheel->rotationSpeed = speed;
    }
}

void Car::OnInputUpdate(float deltaTime, int mods)
{
}

void Car::Render()
{
    scene->RenderMesh(meshes["car"], scene->GetShader("Curve"), modelMatrix, position);
}
