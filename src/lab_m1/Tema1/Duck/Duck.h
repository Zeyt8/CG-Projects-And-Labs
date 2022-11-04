#pragma once

#include "lab_m1/Tema1/GameObject.h"

namespace m1
{
	class Duck : public GameObject
	{
	public:
		Duck(Tema1* scene, float speed = 1);

		void Awake();
		void Start();
		void Update(float deltaTime);

	private:
		float wingsRotation = 0;
		bool wingsDirection = true;
		glm::vec3 movementDir;
		float speed;

		float xMin;
		float xMax;
		float yMin;
		float yMax;
	};
}