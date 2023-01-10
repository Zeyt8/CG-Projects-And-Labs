#include "Rock.h"

using namespace p3;

Rock::Rock(Tema3* scene) : GameObject(scene)
{
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(scene->SourcePrimitiveDir, "sphere.obj");
        Meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(scene->SourceTextureDir, "Rock.png").c_str(), GL_REPEAT);
        Scene->Textures["rock"] = texture;
    }

    SetScale(glm::vec3(0.9f, 0.9f, 0.9f));
}

Rock::~Rock() = default;

void Rock::Render()
{
    glm::mat4 modelMatrix = TranslateMatrix(ModelMatrix, glm::vec3(0, 0.4f, 0));
	Scene->RenderMesh(Meshes["sphere"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["rock"]);
	modelMatrix = TranslateMatrix(modelMatrix, glm::vec3(-0.5f, 0, 0));
	Scene->RenderMesh(Meshes["sphere"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["rock"]);
	modelMatrix = TranslateMatrix(modelMatrix, glm::vec3(1, 0, 0));
	Scene->RenderMesh(Meshes["sphere"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["rock"]);
}