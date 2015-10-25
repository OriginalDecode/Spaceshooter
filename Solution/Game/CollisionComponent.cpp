#include "stdafx.h"

#include <Camera.h>
#include "CollisionComponent.h"
#include "Constants.h"
#include "Entity.h"
#include "GraphicsComponent.h"
#include <Intersection.h>

CollisionComponent::CollisionComponent(Entity& aEntity)
	: Component(aEntity)
{

}

CollisionComponent::~CollisionComponent()
{
}

void CollisionComponent::Initiate(float someRadius)

{
	if (myEntity.GetComponent<GraphicsComponent>() != nullptr)
	{
		SetCollisionRadius(myEntity.GetComponent<GraphicsComponent>()->GetCullingRadius() 
			* globalCollisionRadiusMultiplier);
	}
	else
	{
		SetCollisionRadius(someRadius);
	}
}

void CollisionComponent::Update(float)
{
	mySphere.myCenterPosition = myEntity.myOrientation.GetPos();
}

bool CollisionComponent::CheckCollision(CollisionComponent* anOtherComponent)
{
	return CU::Intersection::SphereVsSphere(mySphere, anOtherComponent->GetSphere());
}