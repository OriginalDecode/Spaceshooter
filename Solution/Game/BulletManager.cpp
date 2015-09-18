#include "stdafx.h"
#include "BulletManager.h"
#include <Camera.h>
#include <Instance.h>
#include "PostMaster.h"

BulletManager::BulletManager()
{
	PostMaster::GetInstance()->Subscribe(eMessageType::ACTIVATE_BULLET, this);

	myBoxBulletCounter = 0;
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
	myBoxBullets.DeleteAll();
}

void BulletManager::Update(float aDeltaTime)
{
	for (size_t i = 0; i < BULLET_AMOUNT; i++)
	{
		if (myIsActiveBoxBullets[i] == true)
		{
			myBoxBullets[i]->Update(aDeltaTime);

		}
	}

	// todo: life counter on bullets
}

void BulletManager::Render(Prism::Camera* aCamera)
{
	for (size_t i = 0; i < BULLET_AMOUNT; i++)
	{
		if (myIsActiveBoxBullets[i] == true)
		{
			myBoxBullets[i]->GetComponent<GraphicsComponent>()->GetInstance()->Render(*aCamera);

		}
	}
}

void BulletManager::ReceiveMessage(const BulletMessage& aMessage)
{
	if (aMessage.GetBulletType() == eBulletType::BOX_BULLET)
	{
		ActivateBoxBullet(aMessage.GetVelocity(), aMessage.GetPosition());
	}
}

void BulletManager::ActivateBoxBullet(CU::Vector3<float> aVelocity, CU::Vector3<float> aPosition)
{
	// has a limited amount of bullets, re-uses old after they've run out
	myBoxBullets[myBoxBulletCounter]->GetComponent<PhysicsComponent>()->Init(aVelocity, aPosition);
	myIsActiveBoxBullets[myBoxBulletCounter] = true;

	myBoxBulletCounter++;

	if (myBoxBulletCounter >= BULLET_AMOUNT)
	{
		myBoxBulletCounter = 0;
	}
}