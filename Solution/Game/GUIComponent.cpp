#include "stdafx.h"
#include <Camera.h>
#include "Constants.h"
#include "GUIComponent.h"
#include <Model2D.h>
#include "SteeringTargetMessage.h"
#include "WaypointMessage.h"

GUIComponent::GUIComponent(Entity& aEntity)
	: Component(aEntity)
{
	mySteeringTarget = new Prism::Model2D;
	myCrosshair = new Prism::Model2D;
	myWaypoint = new Prism::Model2D;
	myCurrentWaypointSprite = new Prism::Model2D;
	myArrow = new Prism::Model2D;
	myCrosshair->Init("Data/resources/texture/crosshair.dds", { 64.f, 64.f }); // the size scales the pic
	mySteeringTarget->Init("Data/resources/texture/crosshair.dds", { 64.f, 64.f });
	myWaypoint->Init("Data/resources/texture/enemy_marker.dds", { 64, 64 });
	myArrow->Init("Data/resources/texture/arrow.dds", { 64, 64 });;
	myCurrentWaypointSprite = nullptr;
	myCamera = nullptr;
}

GUIComponent::~GUIComponent()
{
	delete mySteeringTarget;
	delete myCrosshair;
	delete myWaypoint;
	delete myArrow;
	delete myCurrentWaypointSprite;
	mySteeringTarget = nullptr;
	myCrosshair = nullptr;
	myWaypoint = nullptr;
	myArrow = nullptr;
	myCurrentWaypointSprite = nullptr;
}

void GUIComponent::Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos)
{
	mySteeringTarget->Render(*myCamera, aWindowSize.x / 2.f + mySteeringTargetPosition.x
		, -aWindowSize.y / 2.f - mySteeringTargetPosition.y);
	myCrosshair->Render(*myCamera, aWindowSize.x / 2.f, -(aWindowSize.y / 2.f));

	CU::Vector3<float> toWaypoint = myWaypointPosition - myCamera->GetOrientation().GetPos();
	CU::Vector3<float> forward = myCamera->GetOrientation().GetForward();
	if (CU::Length(toWaypoint) != 0)
	{
		CU::Normalize(toWaypoint);
	}
	if (CU::Length(forward) != 0)
	{
		CU::Normalize(forward);
	}

	float circleAroundPoint = (CU::Dot(toWaypoint, forward));

	CU::Matrix44<float> renderPos; 
	renderPos.SetPos(myWaypointPosition);
	renderPos = renderPos * CU::InverseSimple(myCamera->GetOrientation());
	renderPos = renderPos * myCamera->GetProjection();

	CU::Vector3<float> newRenderPos = renderPos.GetPos();
	newRenderPos /= renderPos.GetPos4().w;

	newRenderPos.x += 1;
	newRenderPos.x /= 2.f;
	newRenderPos.x *= aWindowSize.x;
	newRenderPos.y += 1;
	newRenderPos.y /= 2.f;
	newRenderPos.y *= aWindowSize.y;
	newRenderPos.y -= aWindowSize.y;

	CU::Vector2<float> radius(aWindowSize.x / 2.f, aWindowSize.y / 2.f);
	radius = CU::Vector2<float>(newRenderPos.x, -newRenderPos.y) - radius;
	float length = 0;
	if (radius.x != 0 && radius.y != 0)
	{
		length = CU::Length(radius);
		CU::Normalize(radius);
	}

	if (length > 400.f)
	{
		myCurrentWaypointSprite = myArrow;
		newRenderPos.x = radius.x * 400.f + (aWindowSize.x / 2.f);
		newRenderPos.y = -(radius.y * 400.f + (aWindowSize.y / 2.f));
	}
	else
	{
		myCurrentWaypointSprite = myWaypoint;
	}
	if (circleAroundPoint < 0.f)
	{
		myCurrentWaypointSprite = myArrow;
		newRenderPos.x = -radius.x * 400.f + (aWindowSize.x / 2.f);
		newRenderPos.y = -(-radius.y * 400.f + (aWindowSize.y / 2.f));
	}


	std::string tempX = std::to_string(length);
	SetWindowTextA(GetActiveWindow(), tempX.c_str());

	myCurrentWaypointSprite->Render(*myCamera, newRenderPos.x, newRenderPos.y);
}

void GUIComponent::ReceiveMessage(const SteeringTargetMessage& aMessage)
{
	mySteeringTargetPosition = aMessage.GetPosition();
}

void GUIComponent::ReceiveMessage(const WaypointMessage& aMessage)
{
	myWaypointPosition = aMessage.GetPosition();
}