#include "stdafx.h"

#include <Camera.h>
#include "CollisionComponent.h"
#include "Entity.h"
#include <Instance.h>
#include <Model.h>
#include <ModelLoader.h>

CollisionComponent::~CollisionComponent()
{
	delete myInstance;
	myInstance = nullptr;
}

void CollisionComponent::Initiate(float someRadius)
{
	SetRadius(someRadius);

	myShouldShow = true;

	Prism::ModelProxy* model = Prism::Engine::GetInstance()->GetModelLoader()->LoadLightCube(
		someRadius * 2.f, someRadius * 2.f, someRadius * 2.f);
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