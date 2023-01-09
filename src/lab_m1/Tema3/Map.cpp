#include "Map.h"
#include "Player.h"

p3::Map::Map(Tema3* scene)
{
    const std::vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-0.5f, 0, -0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5f, 0, -0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(0.5f, 0, 0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),
        VertexFormat(glm::vec3(-0.5f, 0, 0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),
    };

    const std::vector<unsigned int> indices =
    {
        0, 1, 2,
        1, 3, 2,
    };

    Texture2D* texture = new Texture2D();
    texture->Load2D(PATH_JOIN(scene->sourceTextureDir, "Snow.png").c_str(), GL_REPEAT);
    scene->textures["snow"] = texture;
}

p3::Map::~Map()
{
}

void p3::Map::Update(float deltaTime)
{
	SetPosition(scene->player->position);
}
