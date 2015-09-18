#pragma once
#include "Entity.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"

#define BULLET_AMOUNT 8

class BulletManager
{
public:
	BulletManager();
	~BulletManager();

private:
	CU::GrowingArray<Entity*> myBoxBullets;
	std::bitset<BULLET_AMOUNT> myIsActiveBoxBullets;

};

