#include "stdafx.h"
#include "ShootingComponent.h"

#include "Entity.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "ShootMessage.h"

void ShootingComponent::Init()
{
	myBullet = new Entity();
	myBullet->AddComponent<GraphicsComponent>()->InitCube(0.5, 0.5, 0.5);
	
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
	myBullet->GetComponent<PhysicsComponent>()->Init(myForward * aMessage.GetSpeed(), myPosition);
	myIsShooting = true;
	myBullet->GetComponent<GraphicsComponent>()->SetSelfRender(myEntity->myCamera);
}

void ShootingComponent::ReceiveMessage(const RefreshOrientationMessage&)
{
	myPosition = myEntity->myOrientation.GetPos();
	myForward = myEntity->myOrientation.GetForward();
}