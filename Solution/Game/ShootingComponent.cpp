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
	myBullet = new Entity();
	myBullet->AddComponent<GraphicsComponent>()->Init("Data/resources/model/asteroids/asteroid__large_placeholder.fbx",
		"Data/effect/BasicEffect.fx");
	
	myBullet->AddComponent<PhysicsComponent>()->Init({ 0, 0, 0 }, { 0, 0, 0 });
	myIsShooting = false;
}


void ShootingComponent::Update(float aDeltaTime)
{
	if (myIsShooting == true)
	{
		myBullet->Update(aDeltaTime);
	}
}

void ShootingComponent::ReceiveMessage(const ShootMessage& aMessage)
{
	PostMaster::GetInstance()->SendMessage(BulletMessage(eBulletType::BOX_BULLET, myForward * aMessage.GetSpeed(), myPosition));
	//myBullet->GetComponent<PhysicsComponent>()->Init(myForward * aMessage.GetSpeed(), myPosition);
	//myIsShooting = true;
	//myBullet->GetComponent<GraphicsComponent>()->SetSelfRender(myEntity->myCamera);
}

void ShootingComponent::ReceiveMessage(const RefreshOrientationMessage&)
{
	myPosition = myEntity->myOrientation.GetPos();
	myForward = myEntity->myOrientation.GetForward();
}