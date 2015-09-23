#include "stdafx.h"
#include "GUIComponent.h"
#include <Model2D.h>
#include <Camera.h>

GUIComponent::GUIComponent()
{
	myMousePointer = new Prism::Model2D;
	myCrosshair = new Prism::Model2D;
	myCrosshair->Init("Data/resources/texture/crosshair.dds", { 128.f, 128.f });
	myMousePointer->Init("Data/resources/texture/crosshair.dds", { 128.f, 128.f });
	myCamera = nullptr;
}

GUIComponent::~GUIComponent()
{
	delete myMousePointer;
	delete myCrosshair;
	myMousePointer = nullptr;
	myCrosshair = nullptr;
}

void GUIComponent::Update(float)
{
	
}

void GUIComponent::Render()
{
	myMousePointer->Render(*myCamera, myMousePos.x, -myMousePos.y);
	myCrosshair->Render(*myCamera, myCenter.x, myCenter.y);
}