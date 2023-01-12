#include "Lamp.h"
#include "Player.h"
#include "Light.h"

using namespace p3;

Lamp::Lamp(Tema3* scene) : GameObject(scene)
{
    {
        Mesh* mesh = new Mesh("cube");
        mesh->LoadMesh(scene->SourcePrimitiveDir, "box.obj");
        Meshes[mesh->GetMeshID()] = mesh;
    }

    ColliderRadius = 0.15f;
}

Lamp::~Lamp()
{
	std::_Erase_remove(Scene->Lights, _light1);
	std::_Erase_remove(Scene->Lights, _light2);
    delete _light1;
    delete _light2;
}

void Lamp::Awake()
{
    _light1 = new Light(LightTypes::Spot, Position + glm::vec3(-0.5f, 5.0f, 0), glm::vec3(1), glm::vec3(RADIANS(90), 0, 0), 0.3f);
    _light2 = new Light(LightTypes::Spot, Position + glm::vec3(0.5f, 5.0f, 0), glm::vec3(1), glm::vec3(RADIANS(90), 0, 0), 0.3f);

    Scene->Lights.push_back(_light1);
    Scene->Lights.push_back(_light2);
}

void Lamp::Update(float deltaTime)
{
    if (glm::abs(Position.z - Scene->PlayerObject->Position.z) > 20)
    {
        Destroy();
    }
}

void Lamp::Render()
{
    glm::mat4 modelMatrix = TranslateMatrix(ModelMatrix, glm::vec3(0, 2.0f, 0));
    modelMatrix = ScaleMatrix(modelMatrix, glm::vec3(0.2f, 4, 0.2f));
	Scene->RenderMesh(Meshes["cube"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["metal"]);

	modelMatrix = TranslateMatrix(ModelMatrix, glm::vec3(0, 4.05f, 0));
    modelMatrix = ScaleMatrix(modelMatrix, glm::vec3(1, 0.1f, 0.5f));
	Scene->RenderMesh(Meshes["cube"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["metal"]);
}
