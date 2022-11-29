#pragma once

#include "Car.h"

namespace p2
{
	class Enemy : public Car
	{
	public:
		Enemy(Tema2* scene, glm::vec3 color);
		~Enemy();

		void Awake() override;
		void Start() override;
		void Update(float deltaTime) override;
		void OnInputUpdate(float deltaTime, int mods) override;
		void Render() override;

	public:
		std::vector<glm::vec3> path;
		int nextNode = 0;
	};
}