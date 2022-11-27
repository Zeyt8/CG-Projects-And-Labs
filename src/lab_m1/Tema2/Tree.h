#pragma once
#include "GameObject.h"

namespace p2
{
	class Tree : public GameObject
	{
	public:
		Tree(Tema2* scene);
		~Tree();

		void Awake() override;
		void Start() override;
		void Update(float deltaTime) override;
	};
}