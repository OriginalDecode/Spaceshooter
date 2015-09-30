#include "stdafx.h"
#include <Camera.h>
#include "GUIComponent.h"
#include <Model2D.h>
#include "SteeringTargetMessage.h"

GUIComponent::GUIComponent()
{
	mySteeringTarget = new Prism::Model2D;
	myCrosshair = new Prism::Model2D;
	myCrosshair->Init("Data/resources/texture/crosshair.dds", { 64.f, 64.f }); // the size scales the pic
	mySteeringTarget->Init("Data/resources/texture/crosshair.dds", { 64.f, 64.f });
	myCamera = nullptr;
}

GUIComponent::~GUIComponent()
{
	delete mySteeringTarget;
	delete myCrosshair;
	mySteeringTarget = nullptr;
	myCrosshair = nullptr;
}

void GUIComponent::Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos)
{
	mySteeringTarget->Render(*myCamera, aWindowSize.x / 2.f + mySteeringTargetPosition.x
		, -aWindowSize.y / 2.f - mySteeringTargetPosition.y);
	myCrosshair->Render(*myCamera, aWindowSize.x / 2.f, -(aWindowSize.y / 2.f));
}

void GUIComponent::ReceiveMessage(const SteeringTargetMessage& aMessage)
{
	mySteeringTargetPosition = aMessage.GetPosition();
}