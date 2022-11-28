#pragma once
#include "GameObject.h"
#include "Cars/Car.h"

namespace p2
{
	class Minimap : public GameObject
	{
	public:
		Minimap(Tema2* scene, Car* car);
		~Minimap();

		void Awake() override;
		void Start() override;
		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void Render() override;

	public:
		Car* car;

	private:
		Camera* camera;
	};
}
