#include "Lamp.h"
#include "Player.h"

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

void Lamp::Update(float deltaTime)
{
    if (glm::abs(Position.z - Scene->PlayerObject->Position.z) > 15)
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
