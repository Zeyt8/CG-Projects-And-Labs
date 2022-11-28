#pragma once

#include "Car.h"

namespace p2
{
	class Enemy : public Car
	{
	public:
		Enemy(Tema2* scene);
		~Enemy();

		void Awake() override;
		void Start() override;
		void Update(float deltaTime) override;
		void OnInputUpdate(float deltaTime, int mods) override;
		void Render() override;
	};
}