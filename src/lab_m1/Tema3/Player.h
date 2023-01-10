#pragma once

#include "GameObject.h"

namespace p3
{
	class Player : public GameObject
	{
	public:
		Player(Tema3* scene);
		~Player();
		void Update(float deltaTime) override;
		void Render() override;
		void OnInputUpdate(float deltaTime, int mods) override;

	private:
		float _speed = 2;
	};
}