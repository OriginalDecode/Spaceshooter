#include "stdafx.h"
#include "GUIComponent.h"
#include <Model2D.h>
#include <Camera.h>

GUIComponent::GUIComponent()
{
	myMousePointer = new Prism::Model2D;
	myCrosshair = new Prism::Model2D;
	myCrosshair->Init("Data/resources/texture/seafloor.dds");
	myMousePointer->Init("Data/resources/texture/seafloor.dds");
	myCamera = nullptr;
}

GUIComponent::~GUIComponent()
{
	delete myMousePointer;
	delete myCrosshair;
	myMousePointer = nullptr;
	myCrosshair = nullptr;
}

void GUIComponent::Update(float aDeltaTime)
{
	myMousePointer->Render(*myCamera, myMousePos.x, -myMousePos.y);
	myCrosshair->Render(*myCamera, myCenter.x, myCenter.y);
}