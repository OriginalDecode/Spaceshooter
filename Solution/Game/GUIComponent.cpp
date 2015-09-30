#include "stdafx.h"
#include "GUIComponent.h"
#include <Model2D.h>
#include <Camera.h>

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

void GUIComponent::Render(const CU::Vector2<int> aScreenCenter, const CU::Vector2<float> aMousePos)
{
	mySteeringTarget->Render(*myCamera, aMousePos.x, -aMousePos.y);
	myCrosshair->Render(*myCamera, aScreenCenter.x / 2.f, -(aScreenCenter.y / 2.f));
}