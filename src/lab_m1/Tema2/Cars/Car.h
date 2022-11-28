#pragma once

#include "../GameObject.h"

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
		void OnInputUpdate(float deltaTime, int mods) override;
		void Render() override;

	protected:
		float speed;
		glm::vec3 color = glm::vec3(0.8f, 0, 0);
	};
}