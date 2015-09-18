#pragma once
#include "Entity.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "BulletMessage.h"

#define BULLET_AMOUNT 8

namespace Prism
{
	class Camera;
}

class BulletManager
{
public:
	BulletManager();
	~BulletManager();

	void Update(float aDeltaTime);

	void Render(Prism::Camera* aCamera);

	void ReceiveMessage(const BulletMessage& aMessage);

	void ActivateBoxBullet(CU::Vector3<float> aVelocity, CU::Vector3<float> aPosition);

private:




	CU::GrowingArray<Entity*> myBoxBullets;
	std::bitset<BULLET_AMOUNT> myIsActiveBoxBullets;
	unsigned int myBoxBulletCounter;

};

