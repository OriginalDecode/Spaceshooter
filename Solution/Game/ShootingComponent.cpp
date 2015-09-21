#include "stdafx.h"
#include "ShootingComponent.h"

#include "Entity.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "ShootMessage.h"
#include "PostMaster.h"
#include "BulletMessage.h"

void ShootingComponent::Init()
{
}

void ShootingComponent::Update(float)
{
}

void ShootingComponent::ReceiveMessage(const ShootMessage& aMessage)
{
	PostMaster::GetInstance()->SendMessage(BulletMessage(eBulletType::BOX_BULLET, myForward * aMessage.GetSpeed(), myPosition));
}

void ShootingComponent::ReceiveMessage(const RefreshOrientationMessage&)
{
	myPosition = myEntity->myOrientation.GetPos();
	myForward = myEntity->myOrientation.GetForward();
}