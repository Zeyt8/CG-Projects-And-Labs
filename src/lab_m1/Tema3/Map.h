#include "GameObject.h"

namespace p3
{
	class Map : public GameObject
	{
	public:
		Map(Tema3* scene);
		~Map();
		void Update(float deltaTime) override;
		void Render() override;
	};
}