#include "GameObject.h"

namespace p3
{
	class Rock : public GameObject
	{
	public:
		Rock(Tema3* scene);
		~Rock();

		void Update(float deltaTime) override;
	};
}