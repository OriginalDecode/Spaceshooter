#include "stdafx.h"

#include <Camera.h>
#include "CollisionComponent.h"
#include "Entity.h"
#include <Instance.h>
#include <Model.h>

CollisionComponent::~CollisionComponent()
{
	delete myInstance;
	myInstance = nullptr;
}

void CollisionComponent::Initiate(float someRadius)
{
	SetRadius(someRadius);
	Prism::Model* model;
	model = new Prism::Model();
	model->InitLightCube(someRadius, someRadius, someRadius);
	myShouldShow = true;
	myInstance = new Prism::Instance(*model);

}

void CollisionComponent::Update(float aDeltaTime)
{
	mySphere.myCenterPosition = myEntity->myOrientation.GetPos();
	myInstance->SetPosition(mySphere.myCenterPosition);
}


void CollisionComponent::ReceiveMessage(const ShootMessage& aMessage)
{

}

void CollisionComponent::Render(Prism::Camera* aCamera)
{
	if (myShouldShow == true)
		myInstance->Render(*aCamera);
}