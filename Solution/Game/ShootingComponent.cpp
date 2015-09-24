#include "stdafx.h"
#include "ShootingComponent.h"

#include "Entity.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "ShootMessage.h"
#include "PostMaster.h"
#include "BulletMessage.h"

ShootingComponent::ShootingComponent()
{
	mySpawningPointOffset = { 0.f, 0.f, 0.f };
}

void ShootingComponent::ReceiveMessage(const ShootMessage&)
{
	// todo: check which weapon is active

	// spawns the bullet a bit in front of player
	CU::Matrix44<float> orientation = myEntity->myOrientation;
	orientation.SetPos(orientation.GetPos() + (orientation.GetForward() * 2.f));
	PostMaster::GetInstance()->SendMessage(BulletMessage(eBulletType::BOX_BULLET, orientation));
}

void ShootingComponent::Init(CU::Vector3<float> aSpawningPointOffset)
{
	mySpawningPointOffset = aSpawningPointOffset;
}