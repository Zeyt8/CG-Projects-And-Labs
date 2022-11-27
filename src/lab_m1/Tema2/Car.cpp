#include "Car.h"
#include "Camera.h"

using namespace p2;

Car::Car(Tema2* scene) : GameObject(scene)
{
    const std::vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-0.5f, 0, 0.5f), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5f, 0, 0.5f), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-0.5f, 1, 0.5f), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5f, 1, 0.5f), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),

        VertexFormat(glm::vec3(-0.5f, 0, -0.5f), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5f, 0, -0.5f), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-0.5f, 1, -0.5f), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5f, 1, -0.5f), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
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
	
    meshes["car"] = new Mesh("car");
    meshes["car"]->InitFromData(vertices, indices);

    CreateMesh("car", vertices, indices);

    speed = 2;
}

Car::~Car() = default;

void Car::Awake()
{
}

void Car::Start()
{
	SetPosition(glm::vec3(22.6f, 0, -16.6f));
    SetRotation(glm::vec3(0, RADIANS(65), 0));
    scene->GetCamera()->RotateThirdPerson_OY(RADIANS(65));
    scene->GetCamera()->RotateThirdPerson_OX(RADIANS(-15));
	SetScale(glm::vec3(1, 1, 2));
}

void Car::Update(float deltaTime)
{
	glm::vec3 moveDir = glm::vec3(scene->GetCamera()->forward.x, 0, scene->GetCamera()->forward.z);
	moveDir = glm::normalize(moveDir);
    if (scene->GetWindow()->KeyHold(GLFW_KEY_W))
    {
		SetPosition(position + moveDir * speed * deltaTime);
    }

    if (scene->GetWindow()->KeyHold(GLFW_KEY_A))
    {
		SetRotation(rotation + glm::vec3(0, 1, 0) * deltaTime);
		scene->GetCamera()->RotateThirdPerson_OY(deltaTime);
    }

    if (scene->GetWindow()->KeyHold(GLFW_KEY_S))
    {
		SetPosition(position - moveDir * speed * deltaTime);
    }

    if (scene->GetWindow()->KeyHold(GLFW_KEY_D))
    {
        SetRotation(rotation - glm::vec3(0, 1, 0) * deltaTime);
        scene->GetCamera()->RotateThirdPerson_OY(-deltaTime);
    }

    scene->RenderMesh(meshes["car"], scene->GetShader("VertexNormal"), modelMatrix);
}
