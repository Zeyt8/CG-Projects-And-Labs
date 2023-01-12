#include "GameObject.h"

namespace p3
{
	class Tree : public GameObject
	{
	public:
		Tree(Tema3* scene);
		~Tree() override;
		void Awake() override;
		void Update(float deltaTime) override;
		void Render() override;
	private:
		Light* _light;
	};
}