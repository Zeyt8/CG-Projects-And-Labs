#include "GameObject.h"

namespace p3
{
	class ObjectSpawner : public GameObject
	{
	public:
		ObjectSpawner(Tema3* scene);
		~ObjectSpawner();
		void Update(float deltaTimeSeconds) override;
	private:
		float _timeBetweenSpawns = 0.5f;
		float _timeUntilSpawn = 0.0f;
	};
}