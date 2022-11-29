#pragma once

#include "GameObject.h"
#include <vector>

namespace p2
{
	class Track : public GameObject
	{
	public:
		Track(Tema2* scene);

		void Awake() override;
		void Start() override;
		void Update(float deltaTime) override;
		void Render() override;

	public:
		static std::vector<glm::vec3> paths[2];
	private:
		std::vector<glm::vec3> points;
	};
}