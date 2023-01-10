#include "GameObject.h"

namespace p3
{
	class Tree : public GameObject
	{
	public:
		Tree(Tema3* scene);
		~Tree();
		void Render() override;
	};
}