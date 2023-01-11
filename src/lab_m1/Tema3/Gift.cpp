#include "Gift.h"
#include "Player.h"

using namespace p3;

Gift::Gift(Tema3* scene) : GameObject(scene)
{
    {
        Mesh* mesh = new Mesh("cube");
        mesh->LoadMesh(scene->SourcePrimitiveDir, "box.obj");
        Meshes[mesh->GetMeshID()] = mesh;
    }

    SetRotation(glm::vec3(RADIANS(30), 0, 0));
    SetScale(glm::vec3(0.8f, 0.8f, 0.8f));
    ColliderRadius = 0.5f;
}

void Gift::Update(float deltaTime)
{
	if (glm::abs(Position.z - Scene->PlayerObject->Position.z) > 15)
	{
		Destroy();
	}
}

void Gift::Render()
{
    glm::mat4 modelMatrix = TranslateMatrix(ModelMatrix, glm::vec3(0, 0.5f, 0));
	Scene->RenderMesh(Meshes["cube"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["gift"]);
}
