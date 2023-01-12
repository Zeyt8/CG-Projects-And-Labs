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
	};
}