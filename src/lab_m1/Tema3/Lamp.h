#pragma once

#include "GameObject.h"

namespace p3
{
	class Lamp : public GameObject
	{
	public:
		Lamp(Tema3* scene);
		~Lamp() override;
		void Awake() override;
		void Update(float deltaTime) override;
		void Render() override;
	private:
		Light* _light1;
		Light* _light2;
	};
}