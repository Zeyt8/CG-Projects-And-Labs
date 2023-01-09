#include "GameObject.h"

namespace p3
{
	class Lamp : public GameObject
	{
	public:
		Lamp(Tema3* scene);
		~Lamp();
		void Update(float deltaTime) override;
	};
}