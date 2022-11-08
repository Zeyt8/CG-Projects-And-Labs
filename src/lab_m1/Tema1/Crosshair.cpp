#include "lab_m1/Tema1/Crosshair.h"

using namespace m1;

Crosshair::Crosshair(Tema1* scene) : GameObject(scene)
{
}

void Crosshair::Awake()
{
    {
        std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.1f, -0.5f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.1f, -0.5f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.1f, 0.5f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.1f, 0.5f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, -0.1f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, -0.1f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 0.1f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, 0.1f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
        };

        std::vector<unsigned int> indices
        {
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7
        };

        meshes["crosshair"] = new Mesh("crosshair");
        meshes["crosshair"]->InitFromData(vertices, indices);

        CreateMesh("crosshair", vertices, indices);
    }
}

void Crosshair::Start()
{

}

void Crosshair::Update(float deltaTime)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec2 pos = glm::vec2(
        (float)scene->GetWindow()->props.cursorPos.x / scene->GetWindow()->GetResolution().x * scene->GetSceneCamera()->GetProjectionInfo().width,
        scene->GetSceneCamera()->GetProjectionInfo().height - (float)scene->GetWindow()->props.cursorPos.y / scene->GetWindow()->GetResolution().y * scene->GetSceneCamera()->GetProjectionInfo().height
    );
    modelMatrix = glm::translate(modelMatrix, glm::vec3(pos.x, pos.y, 0.15f));
    scene->RenderMesh(meshes["crosshair"], scene->GetShader("VertexColor"), modelMatrix);
}