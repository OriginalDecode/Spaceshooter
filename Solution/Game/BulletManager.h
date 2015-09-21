#pragma once
#include <bitset>
#include "Entity.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "BulletMessage.h"
#include "Subscriber.h"

#define BULLET_AMOUNT 8

namespace Prism
{
	class Camera;
}

class BulletManager : Subscriber
{
public:
	BulletManager();
	~BulletManager();

	void Update(float aDeltaTime);

	void Render(Prism::Camera* aCamera);

	void ReceiveMessage(const BulletMessage& aMessage) override;

	void ActivateBoxBullet(CU::Vector3<float> aVelocity, CU::Vector3<float> aPosition);

private:

	CU::GrowingArray<Entity*> myBoxBullets;
	std::bitset<BULLET_AMOUNT> myIsActiveBoxBullets;
	unsigned int myBoxBulletCounter;

};

