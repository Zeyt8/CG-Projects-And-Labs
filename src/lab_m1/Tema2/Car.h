#pragma once

#include "GameObject.h"

namespace p2
{
	class Car : public GameObject
	{
	public:
		Car(Tema2* scene);
		~Car();

		void Awake() override;
		void Start() override;
		void Update(float deltaTime) override;

	protected:
		float speed;
	};
}