#pragma once

#include "GameObject.h"

namespace p3
{
	class Lamp : public GameObject
	{
	public:
		Lamp(Tema3* scene);
		void Update(float deltaTime) override;
		void Render() override;
	};
}