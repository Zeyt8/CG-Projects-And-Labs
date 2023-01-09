#include "Map.h"
#include "Player.h"
#include <iostream>

p3::Map::Map(Tema3* scene) : GameObject(scene)
{
    const std::vector<glm::vec3> vertices
    {
        glm::vec3(-0.5f, 0, -0.5f),
        glm::vec3(-0.5f, 0, 0.5f),
        glm::vec3(0.5f, 0, 0.5f),
        glm::vec3(0.5f, 0, -0.5f)
    };

    const std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,
    };

    const std::vector<glm::vec3> normals =
    {
        glm::vec3(0, 1, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0)
    };

    const std::vector<glm::vec2> tex =
    {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0, 1),
        glm::vec2(1, 1),
        glm::vec2(1, 0)
    };
    
    CreateMesh("snow", vertices, normals, tex, indices);

    Texture2D* texture = new Texture2D();
    texture->Load2D(PATH_JOIN(scene->sourceTextureDir, "Snow.png").c_str(), GL_REPEAT);
    scene->textures["snow"] = texture;

    scale = glm::vec3(10, 1, 10);
}

p3::Map::~Map()
{
}

void p3::Map::Update(float deltaTime)
{
	SetPosition(scene->player->position);
}

void p3::Map::Render()
{
    scene->RenderMesh(meshes["snow"], scene->GetShader("texture"), modelMatrix, scene->textures["snow"]);
}
