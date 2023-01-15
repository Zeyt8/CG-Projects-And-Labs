#pragma once

#include "GameObject.h"

namespace p3
{
	class Gift : public GameObject
	{
	public:
		Gift(Tema3* scene);
		~Gift() override;
		void Awake() override;
		void Update(float deltaTime) override;
		void Render() override;
	private:
		Light* _light;
		std::vector<glm::vec3> _colors = { glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1) };
	};
}