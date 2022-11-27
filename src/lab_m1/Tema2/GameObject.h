#pragma once
#include <unordered_map>
#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "lab_m1/Tema2/Tema2.h"

namespace p2
{
	class GameObject
	{
	public:
		GameObject();
		GameObject(Tema2* scene);
		~GameObject();

		virtual void Awake() = 0;
		virtual void Start() = 0;
		virtual void Update(float deltaTime) = 0;
		bool Destroy = false;
		glm::mat4 modelMatrix = glm::mat4(1);

	public:
		void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
		void SetPosition(glm::vec3 pos);
		void SetRotation(glm::vec3 rot);
		void SetScale(glm::vec3 sc);

	private:
		void GameObject::SetModelMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc);

	public:
		glm::vec3 position = glm::vec3(0);
		glm::vec3 rotation = glm::vec3(0);
		glm::vec3 scale = glm::vec3(1);

		std::unordered_map<std::string, Mesh*> meshes;
		Tema2 *scene = nullptr;
		std::vector<GameObject*> childObjects;
	};
}