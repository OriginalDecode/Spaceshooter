#pragma once
#include "Entity.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "BulletMessage.h"
#include "Subscriber.h"

struct BulletData
{
	eBulletType myType;
	CU::GrowingArray<Entity*> myBullets;
	CU::GrowingArray<bool> myIsActiveBullets;
	int myBulletCounter;
	int myMaxBullet;
};

namespace Prism
{
	class Camera;
}

namespace tinyxml2
{
	class XMLElement;
}

class BulletManager : Subscriber
{
public:
	BulletManager();
	~BulletManager();

	void Update(float aDeltaTime);

	void Render(Prism::Camera* aCamera);

	void ReceiveMessage(const BulletMessage& aMessage) override;

	void ReadFromXML(class XMLReader& aXMLReader, tinyxml2::XMLElement* aBulletElement);

	void ActivateBoxBullet(CU::Vector3<float> aVelocity, CU::Vector3<float> aPosition);

private:

	BulletData myBoxBulletData;

};

