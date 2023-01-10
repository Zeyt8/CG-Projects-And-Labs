#include "Lamp.h"

using namespace p3;

Lamp::Lamp(Tema3* scene) : GameObject(scene)
{
    {
        Mesh* mesh = new Mesh("cube");
        mesh->LoadMesh(scene->SourcePrimitiveDir, "box.obj");
        Meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(scene->SourceTextureDir, "Metal.jpg").c_str(), GL_REPEAT);
        Scene->Textures["metal"] = texture;
    }
}

Lamp::~Lamp() = default;

void Lamp::Render()
{
    glm::mat4 modelMatrix = TranslateMatrix(ModelMatrix, glm::vec3(0, 2.0f, 0));
    modelMatrix = ScaleMatrix(modelMatrix, glm::vec3(0.2f, 4, 0.2f));
	Scene->RenderMesh(Meshes["cube"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["metal"]);

	modelMatrix = TranslateMatrix(ModelMatrix, glm::vec3(0, 4.05f, 0));
    modelMatrix = ScaleMatrix(modelMatrix, glm::vec3(1, 0.1f, 0.5f));
	Scene->RenderMesh(Meshes["cube"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["metal"]);
}