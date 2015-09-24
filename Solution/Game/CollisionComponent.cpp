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

void CollisionComponent::Init()
{
	Prism::Model* model;
	model = new Prism::Model();
	model->InitLightCube(1, 1, 1);
	myShouldShow = true;
	myInstance = new Prism::Instance(*model);

}

void CollisionComponent::Update(float aDeltaTime)
{
	mySphere.myCenterPosition = myEntity->myOrientation.GetPos();
}


void CollisionComponent::ReceiveMessage(const ShootMessage& aMessage)
{

}

void CollisionComponent::Render(Prism::Camera* aCamera)
{
	if (myShouldShow == true)
		myInstance->Render(*aCamera);
}