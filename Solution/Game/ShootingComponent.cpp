#include "stdafx.h"
#include "ShootingComponent.h"

#include "Entity.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "ShootMessage.h"
#include "PostMaster.h"
#include "BulletMessage.h"

void ShootingComponent::ReceiveMessage(const ShootMessage&)
{
	// todo check which bullet is active
	PostMaster::GetInstance()->SendMessage(BulletMessage(eBulletType::BOX_BULLET, myEntity->myOrientation.GetForward(), myEntity->myOrientation));
}