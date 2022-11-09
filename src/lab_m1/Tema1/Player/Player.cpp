#include "lab_m1/Tema1/Player/Player.h"

using namespace m1;

m1::Player::Player(Tema1* scene) : GameObject(scene)
{
}

void Player::Awake()
{
	// health circle
	{
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;
        vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1.0f, 0, 0), glm::vec3(0.2, 0.8, 0.6)));
        for (int i = 0; i < 20; i++)
        {
            vertices.push_back(
                VertexFormat(glm::vec3(glm::cos(i * glm::radians(360.0f / 20)), glm::sin(i * glm::radians(360.0f / 20)), 0), glm::vec3(1.0f, 0, 0), glm::vec3(0.2, 0.8, 0.6))
            );
        }

        for (int i = 0; i < 20; i++)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        indices.push_back(0);
        indices.push_back(20);
        indices.push_back(1);

        meshes["circle"] = new Mesh("circle");
        meshes["circle"]->InitFromData(vertices, indices);

        CreateMesh("circle", vertices, indices);
	}
    // bullet square
    {
        std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(0, 1.0f, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, -0.5f, 0), glm::vec3(0, 1.0f, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 0.5f, 0), glm::vec3(0, 1.0f, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, 0.5f, 0), glm::vec3(0, 1.0f, 0), glm::vec3(0.2, 0.8, 0.6)),
        };

        std::vector<unsigned int> indices
        {
            0, 1, 2,
            0, 2, 3
        };

        meshes["square"] = new Mesh("square");
        meshes["square"]->InitFromData(vertices, indices);

        CreateMesh("square", vertices, indices);
    }
    // score square
    {
        std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0, -0.5, 0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, -0.5, 0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0.5, 0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.8, 0.6)),
        };

        std::vector<unsigned int> indices
        {
            0, 1, 2,
            0, 2, 3
        };

        meshes["score"] = new Mesh("score");
        meshes["score"]->InitFromData(vertices, indices);

        CreateMesh("score", vertices, indices);
    }
}

void Player::Start()
{
}

void Player::Update(float deltaTime)
{
	DrawHealth();
	DrawBullets();
	DrawScore();
}

void Player::DrawHealth()
{
    for (int i = 0; i < health; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(
            modelMatrix,
            glm::vec3(scene->GetSceneCamera()->GetProjectionInfo().width - i - 1, scene->GetSceneCamera()->GetProjectionInfo().height * 0.95f, 0.2f)
        );
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
        scene->RenderMesh(meshes["circle"], scene->GetShader("VertexColor"), modelMatrix);
    }
}

void Player::DrawBullets()
{
    for (int i = 0; i < bullets; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(
            modelMatrix,
            glm::vec3(scene->GetSceneCamera()->GetProjectionInfo().width - i - 1, scene->GetSceneCamera()->GetProjectionInfo().height * 0.89f, 0.2f)
        );
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        scene->RenderMesh(meshes["square"], scene->GetShader("VertexColor"), modelMatrix);
    }
}

void Player::DrawScore()
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(
        modelMatrix,
        glm::vec3(scene->GetSceneCamera()->GetProjectionInfo().width - 3.5f, scene->GetSceneCamera()->GetProjectionInfo().height * 0.82f, 0.2f)
    );

    glm::mat4 modelMatrix1 = glm::scale(modelMatrix, glm::vec3(3, 0.8f, 0.8f));
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    scene->RenderMesh(meshes["score"], scene->GetShader("VertexColor"), modelMatrix1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glm::mat4 modelMatrix2 = glm::scale(modelMatrix, glm::vec3((float)score * 3 / 10, 0.8f, 0.8f));
    scene->RenderMesh(meshes["score"], scene->GetShader("VertexColor"), modelMatrix2);
}