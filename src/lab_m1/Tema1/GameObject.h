#pragma once
#include <unordered_map>
#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "lab_m1/Tema1/Tema1.h"

namespace m1
{
	class GameObject
	{
	public:
		GameObject(Tema1* scene);
		~GameObject();

		virtual void Awake() = 0;
		virtual void Start() = 0;
		virtual void Update(float deltaTime) = 0;
		bool Destroy = false;;

	protected:
		void GameObject::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
		glm::mat4 Translate(glm::mat4 model, glm::vec3 pos);
		glm::mat4 Rotate(glm::mat4 model, float rot, glm::vec3 axis);
		glm::mat4 Scale(glm::mat4 model, glm::vec3 scale);

	protected:
		glm::vec3 position;
		float rotation;
		glm::vec3 scale;

		std::unordered_map<std::string, Mesh*> meshes;
		Tema1 *scene;
		std::vector<GameObject*> childObjects;
	};
}