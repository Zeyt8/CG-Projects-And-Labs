#include "GameObject.h"

namespace p3
{
	class Gift : public GameObject
	{
	public:
		Gift(Tema3* scene);
		~Gift();
		void Render() override;
	};
}