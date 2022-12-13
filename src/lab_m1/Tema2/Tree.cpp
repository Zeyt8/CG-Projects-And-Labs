#include "Tree.h"
#include <vector>

using namespace p2;

Tree::Tree(Tema2* scene) : GameObject(scene)
{
    {
        const std::vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.5f, 0, 0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 0, 0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, 1, 0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 1, 0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),

            VertexFormat(glm::vec3(-0.5f, 0, -0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 0, -0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5f, 1, -0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5f, 1, -0.5f), glm::vec3(0.47f, 0.2f, 0), glm::vec3(0.2, 0.8, 0.6)),

            VertexFormat(glm::vec3(-1, 1, 1), glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 1), glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-1, 3, 1), glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 3, 1), glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(0.2, 0.8, 0.6)),

            VertexFormat(glm::vec3(-1, 1, -1), glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, -1), glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-1, 3, -1), glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 3, -1), glm::vec3(0.1f, 0.4f, 0.1f), glm::vec3(0.2, 0.8, 0.6)),
        };

        const std::vector<unsigned int> indices =
        {
            0, 1, 2,
            1, 3, 2,
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
			1, 5, 7,
			6, 7, 4,
			7, 5, 4,
			0, 4, 1,
			1, 4, 5,
			2, 6, 4,
			0, 2, 4,

            8, 9, 10,
            9, 11, 10,
            10, 11, 15,
            10, 15, 14,
            9, 15, 11,
            9, 13, 15,
            14, 15, 12,
            15, 13, 12,
            8, 12, 9,
            9, 12, 13,
            10, 14, 12,
            8, 10, 12
        };

        CreateMesh("tree", vertices, indices);
    }
}

Tree::~Tree() = default;

void Tree::Awake()
{
}

void Tree::Start()
{
}

void Tree::Update(float deltaTime)
{
}

void Tree::Render()
{
    scene->RenderMesh(meshes["tree"], scene->GetShader("Curve"), modelMatrix);
}
