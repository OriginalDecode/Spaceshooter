#pragma once
#include "Entity.h"
#include "BulletMessage.h"
#include "Subscriber.h"

struct BulletData // holds the data for one type of bullet
{
	eBulletType myType;
	CU::GrowingArray<Entity*> myBullets;
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

	void ActivateBoxBullet(CU::Vector3<float> aVelocity, CU::Vector3<float> aPosition, CU::Vector3<float> aForward);

private:

	BulletData myBoxBulletData;

};

