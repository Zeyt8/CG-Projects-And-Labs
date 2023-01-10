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
		glm::mat4 ModelMatrix = glm::mat4(1);

		Mesh* CreateMesh(const char* name, const std::vector<glm::vec3>& positions,
			const std::vector<glm::vec3>& normals,
			const std::vector<glm::vec2>& texCoords,
			const std::vector<unsigned int>& indices);
		void SetPosition(glm::vec3 pos);
		void SetRotation(glm::vec3 rot);
		void SetScale(glm::vec3 sc);
		static glm::mat4 TranslateMatrix(glm::mat4 modelMatrix, glm::vec3 translation);
		static glm::mat4 ScaleMatrix(glm::mat4 modelMatrix, glm::vec3 scale);
		glm::vec2 GetScreenPosition() const;

	private:
		void SetModelMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc);

	public:
		glm::vec3 Position = glm::vec3(0);
		glm::vec3 Rotation = glm::vec3(0);
		glm::vec3 Scale = glm::vec3(1);
		glm::vec3 Forward = glm::vec3(0, 0, 1);

		std::unordered_map<std::string, Mesh*> Meshes;
		Tema3 *Scene = nullptr;
		GameObject* Parent = nullptr;
		std::vector<GameObject*> Children;
	};
}