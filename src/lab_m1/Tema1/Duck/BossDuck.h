#pragma once

#include "lab_m1/Tema1/Duck/Duck.h"

namespace m1
{
	class BossDuck : public Duck
	{
	public:
		BossDuck(Tema1* scene);

		void Awake();
		void Start();
		void Update(float deltaTime);
	};
}