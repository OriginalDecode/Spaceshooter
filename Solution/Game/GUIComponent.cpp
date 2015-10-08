#include "stdafx.h"
#include <Camera.h>
#include "Constants.h"
#include "GUIComponent.h"
#include "GUINote.h"
#include "MissionNote.h"
#include <Model2D.h>
#include <sstream>

#define CIRCLERADIUS 400.f

GUIComponent::GUIComponent(Entity& aEntity)
	: Component(aEntity)
	, myWaypointActive(false)
	, myEnemiesPosition(16)
	, myReticle(new Prism::Model2D)
	, mySteeringTarget(new Prism::Model2D)
	, myCrosshair(new Prism::Model2D)
	, myEnemiesCursor(new Prism::Model2D)
	, myEnemyMarker(new Prism::Model2D)
	, myEnemyArrow(new Prism::Model2D)
	, myCurrentWaypoint(nullptr)
	, myWaypointArrow(new Prism::Model2D)
	, myWaypointMarker(new Prism::Model2D)
	, myCamera(nullptr)
	, myPowerUpArrow(new Prism::Model2D)
	, myPowerUpMarker(new Prism::Model2D)
	, myPowerUpsCursor(nullptr)
	, myPowerUpPositions(8)
{
	CU::Vector2<float> arrowAndMarkerSize(64, 64);
	myReticle->Init("Data/resources/texture/UI/Navigation_Circle.dds", { 1024.f, 1024.f });
	myCrosshair->Init("Data/resources/texture/UI/Shoting_Crosshair.dds", { 256.f, 256.f }); // the size scales the pic
	mySteeringTarget->Init("Data/resources/texture/UI/Stearing_Crosshair.dds", arrowAndMarkerSize);
	myEnemyMarker->Init("Data/resources/texture/UI/Navigation_Marker_Enemy.dds", arrowAndMarkerSize);
	myEnemyArrow->Init("Data/resources/texture/UI/Navigation_Arrow_Enemy.dds", arrowAndMarkerSize);
	myWaypointMarker->Init("Data/resources/texture/UI/Navigation_Marker_Waypoint.dds", arrowAndMarkerSize);
	myWaypointArrow->Init("Data/resources/texture/UI/Navigation_Arrow_Waypoint.dds", arrowAndMarkerSize);
	myPowerUpMarker->Init("Data/resources/texture/UI/Navigation_Marker_Powerups.dds", arrowAndMarkerSize);
	myPowerUpArrow->Init("Data/resources/texture/UI/Navigation_Arrow_Powerups.dds", arrowAndMarkerSize);
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

void GUIComponent::CalculateAndRender(const CU::Vector3<float>& aPosition, Prism::Model2D* aCurrentModel
	, Prism::Model2D* aArrowModel, Prism::Model2D* aMarkerModel, const CU::Vector2<int> aWindowSize, bool aShowDist)
{
	float halfWidth = aWindowSize.x *0.5f;
	float halfHeight = aWindowSize.y * 0.5f;

	CU::Vector3<float> toTarget = aPosition - myCamera->GetOrientation().GetPos();
	std::stringstream lengthToWaypoint;
	if (aShowDist == true)
	{
		lengthToWaypoint << aPosition.x << " " << aPosition.y << " " << aPosition.z << " " << static_cast<int>(CU::Length(toTarget));
	}
	CU::Vector3<float> forward = myCamera->GetOrientation().GetForward();
	if (CU::Length(toTarget) != 0)
	{
		CU::Normalize(toTarget);
	}
	if (CU::Length(forward) != 0)
	{
		CU::Normalize(forward);
	}

	float circleAroundPoint = (CU::Dot(toTarget, forward));

	CU::Matrix44<float> renderPos;
	renderPos.SetPos(aPosition);
	renderPos = renderPos * CU::InverseSimple(myCamera->GetOrientation());
	renderPos = renderPos * myCamera->GetProjection();

	CU::Vector3<float> newRenderPos = renderPos.GetPos();
	newRenderPos /= renderPos.GetPos4().w;

	newRenderPos += 1.f;
	newRenderPos *= 0.5f;
	newRenderPos.x *= aWindowSize.x;
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
		aCurrentModel = aArrowModel;
		newRenderPos.x = radius.x * CIRCLERADIUS + (halfWidth);
		newRenderPos.y = -(radius.y * CIRCLERADIUS + (halfHeight));
	}
	else
	{
		aCurrentModel = aMarkerModel;
	}
	if (circleAroundPoint < 0.f)
	{
		aCurrentModel = aArrowModel;
		newRenderPos.x = -radius.x * CIRCLERADIUS + (halfWidth);
		newRenderPos.y = -(-radius.y * CIRCLERADIUS + (halfHeight));
	}

	if (aShowDist == true)
	{
		if (aCurrentModel == myWaypointMarker || aCurrentModel == myWaypointArrow)
		{
			Prism::Engine::GetInstance()->PrintDebugText(lengthToWaypoint.str(), { newRenderPos.x - 16.f, newRenderPos.y + 64.f });
		}
		aCurrentModel->Render(*myCamera, newRenderPos.x, newRenderPos.y);
	}
}

void GUIComponent::Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos)
{
	float halfHeight = aWindowSize.y * 0.5f;
	float halfWidth = aWindowSize.x * 0.5f;
	myReticle->Render(*myCamera, halfWidth, -halfHeight);
	mySteeringTarget->Render(*myCamera, halfWidth + mySteeringTargetPosition.x
		, -halfHeight - mySteeringTargetPosition.y);
	myCrosshair->Render(*myCamera, halfWidth, -(halfHeight));

	CalculateAndRender(myWaypointPosition, myCurrentWaypoint, myWaypointArrow, myWaypointMarker, aWindowSize, myWaypointActive);

	for (int i = 0; i < myEnemiesPosition.Size(); ++i)
	{
		CalculateAndRender(myEnemiesPosition[i], myEnemiesCursor, myEnemyArrow, myEnemyMarker, aWindowSize, true);
	}

	for (int i = 0; i < myPowerUpPositions.Size(); ++i)
	{
		CalculateAndRender(myPowerUpPositions[i], myPowerUpsCursor, myPowerUpArrow, myPowerUpMarker, aWindowSize, true);
	}

	myEnemiesPosition.RemoveAll();
	myPowerUpPositions.RemoveAll();
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

void GUIComponent::ReceiveNote(const GUINote& aNote)
{
	switch (aNote.myType)
	{
	case eGUINoteType::WAYPOINT:
		myWaypointPosition = aNote.myPosition;
		break;
	case eGUINoteType::ENEMY:
		myEnemiesPosition.Add(aNote.myPosition);
		break;
	case eGUINoteType::POWERUP:
		myPowerUpPositions.Add(aNote.myPosition);
		break;
	case eGUINoteType::STEERING_TARGET:
		mySteeringTargetPosition = { aNote.myPosition.x, aNote.myPosition.y };
		break;
	default:
		break;
	}
}


