#include "Duck.h"
#include "core/engine.h"
#include "core/gpu/mesh.h"
#include <vector>
#include <glm/gtc/random.hpp>

using namespace m1;

Duck::Duck(Tema1* scene, float speed) : GameObject(scene)
{
    movementDir = glm::vec3(
        (glm::linearRand(-1, 1) > 0 ? 1 : -1) * glm::linearRand(0.2f, 1.0f),
        (glm::linearRand(-1, 1) > 0 ? 1 : -1) * glm::linearRand(0.2f, 1.0f),
        0
    );
    movementDir = glm::normalize(movementDir);
    Duck::speed = speed;
}

void Duck::Awake()
{
    {
        std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(1.5f, 0, 0), glm::vec3(0.42f, 0.19f, 0.05f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, 0.5f, 0), glm::vec3(0.42f, 0.19f, 0.05f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(0.42f, 0.19f, 0.05f), glm::vec3(0.2, 0.8, 0.6)),
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
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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

    scene->RenderMesh(meshes["duckBody"], shaders["VertexColor"], modelMatrix);
    scene->RenderMesh(meshes["head"], shaders["VertexColor"], modelMatrix);
    scene->RenderMesh(meshes["beak"], shaders["VertexColor"], modelMatrix);

    glm::mat4 leftWing = glm::rotate(modelMatrix, glm::radians(wingsRotation), glm::vec3(0, 0, 1));
    scene->RenderMesh(meshes["leftWing"], shaders["VertexColor"], leftWing);

    glm::mat4 rightWing = glm::rotate(modelMatrix, glm::radians(-wingsRotation), glm::vec3(0, 0, 1));
    scene->RenderMesh(meshes["rightWing"], shaders["VertexColor"], rightWing);
}
