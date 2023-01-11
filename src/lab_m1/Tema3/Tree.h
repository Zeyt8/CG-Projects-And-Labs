#include "GameObject.h"

namespace p3
{
	class Tree : public GameObject
	{
	public:
		Tree(Tema3* scene);
		void Update(float deltaTime) override;
		void Render() override;
	};
}