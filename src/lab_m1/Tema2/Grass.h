#pragma once

#include "GameObject.h"

namespace p2
{
	class Grass : public GameObject
	{
	public:
		Grass(Tema2* scene);
		~Grass();

		void Awake() override;
		void Start() override;
		void Update(float deltaTime) override;
		void Render() override;
	};
}