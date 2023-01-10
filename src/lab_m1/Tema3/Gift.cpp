#include "Gift.h"

using namespace p3;

Gift::Gift(Tema3* scene) : GameObject(scene)
{
    {
        Mesh* mesh = new Mesh("cube");
        mesh->LoadMesh(scene->SourcePrimitiveDir, "box.obj");
        Meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(scene->SourceTextureDir, "Gift.jpg").c_str(), GL_REPEAT);
        Scene->Textures["gift"] = texture;
    }

    SetRotation(glm::vec3(RADIANS(30), 0, 0));
    SetScale(glm::vec3(0.8f, 0.8f, 0.8f));
}

Gift::~Gift() = default;

void Gift::Render()
{
    glm::mat4 modelMatrix = TranslateMatrix(ModelMatrix, glm::vec3(0, 0.5f, 0));
	Scene->RenderMesh(Meshes["cube"], Scene->GetShader("texture"), modelMatrix, Scene->Textures["gift"]);
}
