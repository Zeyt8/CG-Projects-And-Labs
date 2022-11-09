#pragma once

#include "lab_m1/Tema1/GameObject.h"
#include <vector>

namespace m1
{
	class Duck : public GameObject
	{
	public:
		Duck(Tema1* scene, float speed = 1);

		void Awake();
		void Start();
		void Update(float deltaTime);

		void Die();
		void Escape();
		virtual void TakeDamage();

		bool IsInBounds(float mouseX, float mouseY);

	protected:
		int health = 1;

	private:
		float wingsRotation = 0;
		bool wingsDirection = true;
		glm::vec3 movementDir;
		float speed;
		int state = 0;

		float xMin;
		float xMax;
		float yMin;
		float yMax;

		std::vector<glm::vec3> colors;
	};
}