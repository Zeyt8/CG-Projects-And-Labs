#pragma once

#include "GameObject.h"

namespace p3
{
	class Player : public GameObject
	{
	public:
		Player(Tema3* scene);
		void Update(float deltaTime) override;
		void Render() override;
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnCollisionEnter(GameObject* other) override;

	public:
		int Score = 0;

	private:
		float _speed = 3;
	};
}