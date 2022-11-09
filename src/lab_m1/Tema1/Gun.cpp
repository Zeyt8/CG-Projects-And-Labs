#include "Gun.h"

m1::Gun::Gun(Tema1* scene) : GameObject(scene)
{
}

void m1::Gun::Awake()
{
    {
        std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.5f, 0, 0), glm::vec3(0.8f, 0.5f, 0.2f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 0, 0), glm::vec3(0.8f, 0.5f, 0.2f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 2, 0), glm::vec3(0.8f, 0.5f, 0.2f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, 2, 0), glm::vec3(0.8f, 0.5f, 0.2f), glm::vec3(0.2, 0.8, 0.6)),
        };

        std::vector<unsigned int> indices
        {
            0, 1, 2,
            0, 2, 3
        };

        meshes["gun"] = new Mesh("gun");
        meshes["gun"]->InitFromData(vertices, indices);

        CreateMesh("gun", vertices, indices);
    }
}

void m1::Gun::Start()
{
}

void m1::Gun::Update(float deltaTime)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(scene->GetSceneCamera()->GetProjectionInfo().width / 2, 1, 0.16f));
    glm::vec2 pos = glm::vec2(
        (float)scene->GetWindow()->props.cursorPos.x / scene->GetWindow()->GetResolution().x * scene->GetSceneCamera()->GetProjectionInfo().width,
        scene->GetSceneCamera()->GetProjectionInfo().height - (float)scene->GetWindow()->props.cursorPos.y / scene->GetWindow()->GetResolution().y * scene->GetSceneCamera()->GetProjectionInfo().height
    );
    glm::vec2 dir = glm::normalize(pos - glm::vec2(scene->GetSceneCamera()->GetProjectionInfo().width / 2, 1));
    float angle = glm::acos(glm::dot(glm::vec2(0, 1), dir));
    angle *= dir.x > 0 ? -1 : 1;
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 0, 1));
    scene->RenderMesh(meshes["gun"], scene->GetShader("VertexColor"), modelMatrix);
}
