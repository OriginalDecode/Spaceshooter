#pragma once
#include "Entity.h"
#include "BulletMessage.h"
#include "Subscriber.h"

struct BulletData // holds the data for one type of bullet
{
	CU::GrowingArray<Entity*> myBullets;
	eBulletType myType;
	float mySpeed;
	int myBulletCounter;
	int myMaxBullet;
};

namespace Prism
{
	class Camera;
	class Instance;
}

namespace tinyxml2
{
	class XMLElement;
}

class BulletManager : public Subscriber
{
public:
	BulletManager();
	~BulletManager();

	void ReadFromXML(const std::string aFilePath);
	void Update(float aDeltaTime);
	void ReceiveMessage(const BulletMessage& aMessage) override;

	CU::GrowingArray<Prism::Instance*>& GetInstances();
	CU::GrowingArray<Entity*>& GetEntities();

private:
	void ActivateBoxBullet(const CU::Matrix44<float>& anOrientation);

	BulletData* myBoxBulletData;
	BulletData* myBoxBullet2Data;

	// temps!!
	CU::GrowingArray<Prism::Instance*> myInstances;
	CU::GrowingArray<Entity*> myEntities;

};

