#include "Grass.h"

using namespace p2;

Grass::Grass(Tema2* scene) : GameObject(scene)
{
    int res = 300;
    std::vector<VertexFormat> vertices;
    for (int i = 0; i < res; i++)
    {
	    for (int j = 0; j < res; j++)
	    {
			vertices.emplace_back(glm::vec3(i * 2.0f - res / 2, 0, j * 2.0f - res / 2), glm::vec3(0.09f, 0.82f, 0.29f));
	    }
    }

    std::vector<unsigned int> indices;
    for (int i = 0; i < res - 1; i++)
    {
        for (int j = 0; j < res - 1; j++)
        {
			indices.push_back(i + j * res);
			indices.push_back(i + 1 + j * res);
			indices.push_back(i + (j + 1) * res);
            indices.push_back(i + 1 + j * res);
            indices.push_back(i + 1 + (j + 1) * res);
            indices.push_back(i + (j + 1) * res);
        }
    }

    meshes["grass"] = new Mesh("grass");
    meshes["grass"]->InitFromData(vertices, indices);

    CreateMesh("grass", vertices, indices);

    SetPosition(glm::vec3(0, -0.05f, 0));
}

Grass::~Grass() = default;

void Grass::Awake()
{

}

void Grass::Start()
{
}

void Grass::Update(float deltaTime)
{
}

void Grass::Render()
{
	scene->RenderMesh(meshes["grass"], scene->GetShader("Curve"), modelMatrix, position);
}
