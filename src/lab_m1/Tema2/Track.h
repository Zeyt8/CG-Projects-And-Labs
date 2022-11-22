#pragma once

#include "lab_m1/Tema2/GameObject.h"
#include <vector>

namespace p2
{
	class Track : public GameObject
	{
	public:
		Track(Tema2* scene);

		void Awake();
		void Start();
		void Update(float deltaTime);

	private:
		std::vector<glm::vec3> points;
	};
}