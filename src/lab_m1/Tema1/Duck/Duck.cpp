#include "lab_m1/Tema1/Duck/Duck.h"
#include "core/engine.h"
#include "core/gpu/mesh.h"
#include <vector>
#include <glm/gtc/random.hpp>
#include <glm/gtx/matrix_decompose.hpp>

using namespace m1;

Duck::Duck(Tema1* scene, float speed) : GameObject(scene)
{
    movementDir = glm::vec3(
        (glm::linearRand(-1, 1) > 0 ? 1 : -1) * glm::linearRand(0.1f, 1.0f),
        glm::linearRand(0.3f, 0.9f),
        0
    );
    scale = glm::vec3(1.5f, 1.5f, 1.5f);
    Duck::speed = speed;
    if (movementDir.x < 0)
    {
        scale.x *= -1;
    }
    movementDir = glm::normalize(movementDir);
    position = glm::vec3(glm::linearRand(5, 20), 0, 0);

    xMin = 1;
    yMin = 0.5f;
    xMax = scene->GetSceneCamera()->GetProjectionInfo().width - 1;
    yMax = scene->GetSceneCamera()->GetProjectionInfo().height - 0.5f;

    colors.push_back(glm::vec3(0.42f, 0.19f, 0.05f));
    colors.push_back(glm::vec3(0.73f, 0.13f, 0.06f));
    colors.push_back(glm::vec3(0.54f, 0.55f, 0.02f));
}

void Duck::Awake()
{
    {
        int r = rand() % colors.size();
        std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(1.5f, 0, 0), colors[r], glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, 0.5f, 0), colors[r], glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, -0.5f, 0), colors[r], glm::vec3(0.2, 0.8, 0.6)),
        };

        std::vector<unsigned int> indices
        {
            0, 1, 2,
        };

        meshes["duckBody"] = new Mesh("duckBody");
        meshes["duckBody"]->InitFromData(vertices, indices);

        CreateMesh("duckBody", vertices, indices);
    }
    {
        std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.42f, 0.19f, 0.05f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 0, 0), glm::vec3(0.42f, 0.19f, 0.05f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.25f, 1, 0), glm::vec3(0.42f, 0.19f, 0.05f), glm::vec3(0.2, 0.8, 0.6)),
        };

        std::vector<unsigned int> indices
        {
            0, 1, 2,
        };

        meshes["leftWing"] = new Mesh("leftWing");
        meshes["leftWing"]->InitFromData(vertices, indices);

        CreateMesh("leftWing", vertices, indices);
    }
    {
        std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.42f, 0.19f, 0.05f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 0, 0), glm::vec3(0.42f, 0.19f, 0.05f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.25f, -1, 0), glm::vec3(0.42f, 0.19f, 0.05f), glm::vec3(0.2, 0.8, 0.6)),
        };

        std::vector<unsigned int> indices
        {
            0, 2, 1,
        };

        meshes["rightWing"] = new Mesh("rightWing");
        meshes["rightWing"]->InitFromData(vertices, indices);

        CreateMesh("rightWing", vertices, indices);
    }
    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;
        vertices.push_back(VertexFormat(glm::vec3(1.3f, 0, 0.01f), glm::vec3(0.08f, 0.48f, 0.19f), glm::vec3(0.2, 0.8, 0.6)));
        for (int i = 0; i < 20; i++)
        {
            vertices.push_back(
                VertexFormat(glm::vec3(glm::cos(i * glm::radians(360.0f / 20)) / 3 + 1.3f, glm::sin(i * glm::radians(360.0f / 20)) / 3, 0.01f), glm::vec3(0.08f, 0.48f, 0.19f), glm::vec3(0.2, 0.8, 0.6))
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

        meshes["head"] = new Mesh("head");
        meshes["head"]->InitFromData(vertices, indices);

        CreateMesh("head", vertices, indices);
    }
    {
        std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(1.5f, -0.2f, 0), glm::vec3(0.78f, 0.7f, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(2, 0, 0), glm::vec3(0.78f, 0.7f, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1.5f, 0.2f, 0), glm::vec3(0.78f, 0.7f, 0), glm::vec3(0.2, 0.8, 0.6)),
        };

        std::vector<unsigned int> indices
        {
            0, 1, 2,
        };

        meshes["beak"] = new Mesh("beak");
        meshes["beak"]->InitFromData(vertices, indices);

        CreateMesh("beak", vertices, indices);
    }
}

void Duck::Start()
{
}

void Duck::Update(float deltaTime)
{
    position += movementDir * speed * deltaTime;
    if (wingsDirection)
    {
        wingsRotation += deltaTime * 40;
        if (wingsRotation > 30)
        {
            wingsDirection = false;
        }
    }
    else
    {
        wingsRotation -= deltaTime * 40;
        if (wingsRotation < -10)
        {
            wingsDirection = true;
        }
    }

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, scale);

    scene->RenderMesh(meshes["duckBody"], scene->GetShader("VertexColor"), modelMatrix);
    scene->RenderMesh(meshes["head"], scene->GetShader("VertexColor"), modelMatrix);
    scene->RenderMesh(meshes["beak"], scene->GetShader("VertexColor"), modelMatrix);

    glm::mat4 leftWing = glm::rotate(modelMatrix, glm::radians(wingsRotation), glm::vec3(0, 0, 1));
    scene->RenderMesh(meshes["leftWing"], scene->GetShader("VertexColor"), leftWing);

    glm::mat4 rightWing = glm::rotate(modelMatrix, glm::radians(-wingsRotation), glm::vec3(0, 0, 1));
    scene->RenderMesh(meshes["rightWing"], scene->GetShader("VertexColor"), rightWing);

    if (position.x > xMax && movementDir.x > 0)
    {
        movementDir = glm::reflect(movementDir, glm::vec3(-1, 0, 0));
        scale.x *= -1;
    }
    else if (position.x < xMin && movementDir.x < 0)
    {
        movementDir =  glm::reflect(movementDir, glm::vec3(1, 0, 0));
        scale.x *= -1;
    }
    if (position.y > yMax && movementDir.y > 0)
    {
        if (state == 2)
        {
            Destroy = true;
            scene->isDuckInScene = false;
        }
        movementDir = glm::reflect(movementDir, glm::vec3(0, -1, 0));
    }
    else if (position.y < yMin && movementDir.y < 0)
    {
        if (state == 1)
        {
            Destroy = true;
            scene->isDuckInScene = false;
        }
        movementDir = glm::reflect(movementDir, glm::vec3(0, 1, 0));
    }
}

void Duck::Die()
{
    state = 1;
    scale.x = glm::abs(scale.x);
    rotation = -90;
    movementDir = glm::vec3(0, -1, 0);
    speed *= 2;
}

void Duck::Escape()
{
    if (state != 0)
    {
        return;
    }
    state = 2;
    scale.x = glm::abs(scale.x);
    rotation = 90;
    movementDir = glm::vec3(0, 1, 0);
    speed *= 2;
}

bool Duck::IsInBounds(float mouseX, float mouseY)
{
    float xMin = position.x - (scale.x > 0 ? 0.6f : 1.6f);
    float xMax = position.x + (scale.x > 0 ? 1.6f : 0.6f);
    float yMin = position.y - 0.6f;
    float yMax = position.y + 0.6f;

    return mouseX >= xMin && mouseX <= xMax && mouseY >= yMin && mouseY <= yMax;
}