#pragma once
#include <unordered_map>
#include "core/gpu/mesh.h"
#include "lab_m1/Tema3/Tema3.h"

namespace p3
{
	class GameObject
	{
	public:
		GameObject();
		GameObject(Tema3* scene);
		~GameObject();

		virtual void Awake();
		virtual void Start();
		virtual void Update(float deltaTime);
		virtual void LateUpdate(float deltaTime);
		virtual void OnInputUpdate(float deltaTime, int mods);
		virtual void Render();
		bool Destroy = false;
		glm::mat4 modelMatrix = glm::mat4(1);

		Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
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
		Tema3 *scene = nullptr;
		GameObject* parent = nullptr;
		std::vector<GameObject*> children;
	};
}