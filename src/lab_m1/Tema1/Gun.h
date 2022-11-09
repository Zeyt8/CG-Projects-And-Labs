#pragma once

#include "lab_m1/Tema1/GameObject.h"

namespace m1
{
	class Gun : public GameObject
	{
	public:
		Gun(Tema1* scene);

		void Awake();
		void Start();
		void Update(float deltaTime);
	};
}