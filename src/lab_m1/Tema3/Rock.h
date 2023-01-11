#pragma once

#include "GameObject.h"

namespace p3
{
	class Rock : public GameObject
	{
	public:
		Rock(Tema3* scene);
		void Update(float deltaTime) override;
		void Render() override;
	};
}