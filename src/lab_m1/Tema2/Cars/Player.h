#pragma once

#include "Car.h"

namespace p2
{
	class Player : public Car
	{
	public:
		Player(Tema2* scene, glm::vec3 color);
		~Player();

		void Awake() override;
		void Start() override;
		void Update(float deltaTime) override;
		void OnInputUpdate(float deltaTime, int mods) override;
		void Render() override;
	};
}