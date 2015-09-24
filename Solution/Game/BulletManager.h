#pragma once
#include "Entity.h"
#include "BulletMessage.h"
#include "Subscriber.h"

struct BulletData // holds the data for one type of bullet
{
	CU::GrowingArray<Entity*> myBullets;
	eBulletType myType;
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

	void ReadFromXML(class XMLReader& aXMLReader, tinyxml2::XMLElement* aBulletElement);
	void Update(float aDeltaTime);
	void Render(Prism::Camera* aCamera);

	void ReceiveMessage(const BulletMessage& aMessage) override;

private:
	void ActivateBoxBullet(const CU::Vector3<float>& aVelocity, const CU::Matrix44<float>& anOrientation);

	BulletData myBoxBulletData;

};

