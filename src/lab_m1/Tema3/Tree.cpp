#include "Tree.h"
#include "Player.h"

using namespace p3;

Tree::Tree(Tema3* scene) : GameObject(scene)
{
    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(scene->SourcePrimitiveDir, "cone.obj");
        Meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("trunk");
        mesh->LoadMesh(scene->SourcePrimitiveDir, "box.obj");
        Meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(scene->SourceTextureDir, "Wood.jpg").c_str(), GL_REPEAT);
        Scene->Textures["wood"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(scene->SourceTextureDir, "Tree.png").c_str(), GL_REPEAT);
        Scene->Textures["tree"] = texture;
    }

    ColliderRadius = 0.35f;
}

void Tree::Update(float deltaTime)
{
    if (glm::abs(Position.z - Scene->PlayerObject->Position.z) > 15)
    {
        Destroy();
    }
}

void Tree::Render()
{
    glm::mat4 trunkModelMatrix = ModelMatrix;
    trunkModelMatrix = TranslateMatrix(trunkModelMatrix, glm::vec3(0, 0.5f, 0));
    trunkModelMatrix = ScaleMatrix(trunkModelMatrix, glm::vec3(0.5f, 2, 0.5f));
    Scene->RenderMesh(Meshes["trunk"], Scene->GetShader("texture"), trunkModelMatrix, Scene->Textures["wood"]);

    glm::mat4 coneModelMatrix = ModelMatrix;
    coneModelMatrix = TranslateMatrix(coneModelMatrix, glm::vec3(0, 2, 0));
    Scene->RenderMesh(Meshes["cone"], Scene->GetShader("texture"), coneModelMatrix, Scene->Textures["tree"]);
}
