#include "ObjectSpawner.h"
#include "Tree.h"
#include "Lamp.h"
#include "Rock.h"
#include "Gift.h"
#include "Player.h"

using namespace p3;

ObjectSpawner::ObjectSpawner(Tema3* scene) : GameObject(scene)
{
}

ObjectSpawner::~ObjectSpawner() = default;

void ObjectSpawner::Update(float deltaTimeSeconds)
{
	if (_timeUntilSpawn <= 0)
	{
		_timeUntilSpawn = _timeBetweenSpawns;
		GameObject* go = nullptr;
		switch (rand() % 4)
		{
		case 0:
			go = new Tree(Scene);
			break;
		case 1:
			go = new Lamp(Scene);
			break;
		case 2:
			go = new Rock(Scene);
			break;
		case 3:
			go = new Gift(Scene);
			break;
		default:
			break;
		}
		if (go != nullptr)
		{
			go->SetPosition(Scene->PlayerObject->Position + Tema3::ConvertToTiltedPlane(glm::vec3((rand() % 20) - 10, 0, 10)));
			Scene->AddObject(go);
		}
	}
	_timeUntilSpawn -= deltaTimeSeconds;
}
