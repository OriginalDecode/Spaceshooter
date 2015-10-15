#include "stdafx.h"

#include <Camera.h>
#include "CollisionComponent.h"
#include "Entity.h"

CollisionComponent::CollisionComponent(Entity& aEntity)
	: Component(aEntity)
{

}

CollisionComponent::~CollisionComponent()
{
}

void CollisionComponent::Initiate(float someRadius)
{
	SetRadius(someRadius);


}

void CollisionComponent::Update(float)
{
	mySphere.myCenterPosition = myEntity.myOrientation.GetPos();
}
