#include "BossDuck.h"

m1::BossDuck::BossDuck(Tema1* scene) : Duck(scene, 4)
{
	scale *= 2;
	health = 3;
}

void m1::BossDuck::Awake()
{
	Duck::Awake();
    {
        std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0, -0.5f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, -0.5f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0.5f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 0.5f, 0), glm::vec3(1, 0, 0), glm::vec3(0.2, 0.8, 0.6)),
        };

        std::vector<unsigned int> indices
        {
            0, 1, 2,
            0, 2, 3,
        };

        meshes["bar"] = new Mesh("bar");
        meshes["bar"]->InitFromData(vertices, indices);

        CreateMesh("bar", vertices, indices);
    }
}

void m1::BossDuck::Start()
{
	Duck::Start();
}

void m1::BossDuck::Update(float deltaTime)
{
	Duck::Update(deltaTime);

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(
        modelMatrix,
        glm::vec3(scene->GetSceneCamera()->GetProjectionInfo().width / 10, scene->GetSceneCamera()->GetProjectionInfo().height / 5, 0.2f));
    glm::mat4 newMatrix = glm::scale(modelMatrix, glm::vec3(scene->GetSceneCamera()->GetProjectionInfo().width * 0.8f, 0.5f, 1));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    scene->RenderMesh(meshes["bar"], scene->GetShader("VertexColor"), newMatrix);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glm::mat4 newMat2 = glm::scale(modelMatrix, glm::vec3(scene->GetSceneCamera()->GetProjectionInfo().width * 0.8f * health / 3, 0.5f, 1));
    scene->RenderMesh(meshes["bar"], scene->GetShader("VertexColor"), newMat2);
}
