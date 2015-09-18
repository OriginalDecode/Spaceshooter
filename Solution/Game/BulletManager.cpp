#include "stdafx.h"
#include "BulletManager.h"

BulletManager::BulletManager()
{
	myBoxBullets.Init(BULLET_AMOUNT);
	myIsActiveBoxBullets.reset();

	for (size_t i = 0; i < BULLET_AMOUNT; i++)
	{
		Entity* newEntity = new Entity();
		newEntity->AddComponent<GraphicsComponent>()->InitCube(0.5, 0.5, 0.5);
		newEntity->GetComponent<GraphicsComponent>()->SetPosition({0, 0, 0});
		newEntity->AddComponent<PhysicsComponent>()->Init({ 0, 0, 0 }, { 0, 0, 0 });
		myBoxBullets.Add(newEntity);
		myIsActiveBoxBullets[i] = false;
	}
}

BulletManager::~BulletManager()
{
}
