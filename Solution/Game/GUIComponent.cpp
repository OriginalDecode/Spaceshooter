#include "stdafx.h"
#include <Camera.h>
#include "Constants.h"
#include "EnemiesTargetNote.h"
#include "GUIComponent.h"
#include "MissionNote.h"
#include <Model2D.h>
#include "SteeringTargetNote.h"
#include <sstream>
#include "WaypointNote.h"

#define CIRCLERADIUS 400.f

GUIComponent::GUIComponent(Entity& aEntity)
	: Component(aEntity)
	, myWaypointActive(false)
{
	myReticle = new Prism::Model2D;
	mySteeringTarget = new Prism::Model2D;
	myCrosshair = new Prism::Model2D;
	myEnemyMarker = new Prism::Model2D;
	myCurrentWaypoint = new Prism::Model2D;
	myEnemyArrow = new Prism::Model2D;
	myWaypointMarker = new Prism::Model2D;
	myWaypointArrow = new Prism::Model2D;
	myReticle->Init("Data/resources/texture/UI/Navigation_Circle.dds", { 512.f, 512.f });
	myCrosshair->Init("Data/resources/texture/UI/Shoting_Crosshair.dds", { 256.f, 256.f }); // the size scales the pic
	mySteeringTarget->Init("Data/resources/texture/UI/Stearing_Crosshair.dds", { 64.f, 64.f });
	myEnemyMarker->Init("Data/resources/texture/UI/Navigation_Marker_Enemy.dds", { 64, 64 });
	myEnemyArrow->Init("Data/resources/texture/UI/Navigation_Arrow_Enemy.dds", { 64, 64 });
	myWaypointMarker->Init("Data/resources/texture/UI/Navigation_Marker_Waypoint.dds", { 64, 64 });
	myWaypointArrow->Init("Data/resources/texture/UI/Navigation_Arrow_Waypoint.dds", { 64, 64 });
	myCurrentWaypoint = nullptr;
	myCamera = nullptr;

	myEnemiesPosition.Init(8);
	myEnemiesCursor = new Prism::Model2D;
}	 

GUIComponent::~GUIComponent()
{
	delete mySteeringTarget;
	delete myCrosshair;
	delete myEnemyMarker;
	delete myEnemyArrow;
	delete myCurrentWaypoint;
	delete myWaypointArrow;
	delete myWaypointMarker;
	myWaypointMarker = nullptr;
	myWaypointArrow = nullptr;
	mySteeringTarget = nullptr;
	myCrosshair = nullptr;
	myEnemyMarker = nullptr;
	myEnemyArrow = nullptr;
	myCurrentWaypoint = nullptr;
}

void GUIComponent::Update(float aDeltaTime)
{
	aDeltaTime;
	
}

void GUIComponent::Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos)
{
	float halfHeight = aWindowSize.y * 0.5f;
	float halfWidth = aWindowSize.x * 0.5f;
	mySteeringTarget->Render(*myCamera, halfWidth + mySteeringTargetPosition.x
		, -halfHeight - mySteeringTargetPosition.y);
	myCrosshair->Render(*myCamera, halfWidth, -(halfHeight));
	std::stringstream lengthToWaypoint;
	CU::Vector3<float> toWaypoint = myWaypointPosition - myCamera->GetOrientation().GetPos();
	lengthToWaypoint << myWaypointPosition.x << " " << myWaypointPosition.y << " " << myWaypointPosition.z << " " << static_cast<int>(CU::Length(toWaypoint));
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

	CU::Vector2<float> radius(halfWidth, halfHeight);
	radius = CU::Vector2<float>(newRenderPos.x, -newRenderPos.y) - radius;
	float length = 0;
	if (radius.x != 0 && radius.y != 0)
	{
		length = CU::Length(radius);
		CU::Normalize(radius);
	}

	if (length > CIRCLERADIUS)
	{
		myCurrentWaypoint = myWaypointArrow;
		newRenderPos.x = radius.x * CIRCLERADIUS + (halfWidth);
		newRenderPos.y = -(radius.y * CIRCLERADIUS + (halfHeight));
	}
	else
	{
		myCurrentWaypoint = myWaypointMarker;
	}
	if (circleAroundPoint < 0.f)
	{
		myCurrentWaypoint = myWaypointArrow;
		newRenderPos.x = -radius.x * CIRCLERADIUS + (halfWidth);
		newRenderPos.y = -(-radius.y * CIRCLERADIUS + (halfHeight));
	}

	if (myWaypointActive == true)
	{
		Prism::Engine::GetInstance()->PrintDebugText(lengthToWaypoint.str(), { newRenderPos.x - 16.f, newRenderPos.y + 64.f });
		myCurrentWaypoint->Render(*myCamera, newRenderPos.x, newRenderPos.y);
	}

	for (int i = 0; i < myEnemiesPosition.Size(); ++i)
	{
		CU::Vector3<float> toEnemy = myEnemiesPosition[i] - myCamera->GetOrientation().GetPos();
		CU::Vector3<float> forward = myCamera->GetOrientation().GetForward();
		if (CU::Length(toEnemy) != 0)
		{
			CU::Normalize(toEnemy);
		}
		if (CU::Length(forward) != 0)
		{
			CU::Normalize(forward);
		}

		float circleAroundPoint = (CU::Dot(toEnemy, forward));

		CU::Matrix44<float> renderPos;
		renderPos.SetPos(myEnemiesPosition[i]);
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

		CU::Vector2<float> radius(halfWidth, halfHeight);
		radius = CU::Vector2<float>(newRenderPos.x, -newRenderPos.y) - radius;
		float length = 0;
		if (radius.x != 0 && radius.y != 0)
		{
			length = CU::Length(radius);
			CU::Normalize(radius);
		}

		if (length > CIRCLERADIUS)
		{
			myEnemiesCursor = myEnemyArrow;
			newRenderPos.x = radius.x * CIRCLERADIUS + (aWindowSize.x / 2.f);
			newRenderPos.y = -(radius.y * CIRCLERADIUS + (halfHeight));
		}
		else
		{
			myEnemiesCursor = myEnemyMarker;
		}
		if (circleAroundPoint < 0.f)
		{
			myEnemiesCursor = myEnemyArrow;
			newRenderPos.x = -radius.x * CIRCLERADIUS + (aWindowSize.x / 2.f);
			newRenderPos.y = -(-radius.y * CIRCLERADIUS + (halfHeight));
		}

		myEnemiesCursor->Render(*myCamera, newRenderPos.x, newRenderPos.y);
	}

	myReticle->Render(*myCamera, halfWidth, -halfHeight);

	myEnemiesPosition.RemoveAll();
}

void GUIComponent::ReceiveNote(const EnemiesTargetNote& aMessage)
{
	myEnemiesPosition.Add(aMessage.myPosition);
}

void GUIComponent::ReceiveNote(const MissionNote& aMessage)
{
	if (aMessage.myEvent == eMissionEvent::START && aMessage.myType == eMissionType::WAYPOINT)
	{
		myWaypointActive = true;
	}
	else
	{
		myWaypointActive = false;
	}
}

void GUIComponent::ReceiveNote(const SteeringTargetNote& aMessage)
{
	mySteeringTargetPosition = aMessage.myPosition;
}

void GUIComponent::ReceiveNote(const WaypointNote& aMessage)
{
	myWaypointPosition = aMessage.myPosition;
}



