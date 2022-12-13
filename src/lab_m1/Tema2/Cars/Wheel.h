#pragma once

#include "../GameObject.h"

namespace p2
{
	class Wheel : public GameObject
	{
	public:
		Wheel(Tema2* scene) : GameObject(scene)
		{
            glm::vec3 color = glm::vec3(0.1f);
            const std::vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(-0.15f, -0.2f, 0.2f), color, glm::vec3(0.2, 0.8, 0.6)),
                VertexFormat(glm::vec3(0.15f, -0.2f, 0.2f), color, glm::vec3(0.2, 0.8, 0.6)),
                VertexFormat(glm::vec3(-0.15f, 0.2f, 0.2f), color, glm::vec3(0.2, 0.8, 0.6)),
                VertexFormat(glm::vec3(0.15f, 0.2f, 0.2f), color, glm::vec3(0.2, 0.8, 0.6)),
				
                VertexFormat(glm::vec3(-0.15f, -0.2f, -0.2f), color, glm::vec3(0.2, 0.8, 0.6)),
                VertexFormat(glm::vec3(0.15f, -0.2f, -0.2f), color, glm::vec3(0.2, 0.8, 0.6)),
                VertexFormat(glm::vec3(-0.15f, 0.2f, -0.2f), color, glm::vec3(0.2, 0.8, 0.6)),
                VertexFormat(glm::vec3(0.15f, 0.2f, -0.2f), color, glm::vec3(0.2, 0.8, 0.6)),
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
            };

			CreateMesh("wheel", vertices, indices);
		}

		~Wheel() = default;

		void Awake() override
		{
		}

		void Start() override {}

		void Update(float deltaTime) override
		{
			const glm::mat4 mm = parent->modelMatrix * modelMatrix;
			SetRotation(rotation - glm::vec3(deltaTime * rotationSpeed, 0, 0));
			scene->RenderMesh(meshes["wheel"], scene->GetShader("Curve"), mm);
		}

	public:
		float rotationSpeed = 0;
	};
}