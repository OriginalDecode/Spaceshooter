#include "stdafx.h"
#include <Camera.h>
#include "BulletCollisionToGUIMessage.h"
#include "Constants.h"
#include "ConversationMessage.h"
#include "DefendMessage.h"
#include "Entity.h"
#include "GUIComponent.h"
#include "GUINote.h"
#include "HealthComponent.h"
#include "HealthNote.h"
#include "MissionNote.h"
#include <Model2D.h>
#include "PostMaster.h"
#include "PropComponent.h"
#include "ResizeMessage.h"
#include "ShieldNote.h"
#include <sstream>
#include <XMLReader.h>

#define CIRCLERADIUS 400.f

GUIComponent::GUIComponent(Entity& aEntity)
	: Component(aEntity)
	, myWaypointActive(false)
	, myEnemiesPosition(16)
	, myReticle(new Prism::Model2D)
	, mySteeringTarget(new Prism::Model2D)
	, myCrosshair(new Prism::Model2D)
	, myEnemyMarker(new Prism::Model2D)
	, myEnemyArrow(new Prism::Model2D)
	, myWaypointArrow(new Prism::Model2D)
	, myWaypointMarker(new Prism::Model2D)
	, myCamera(nullptr)
	, myPowerUpArrow(new Prism::Model2D)
	, myPowerUpMarker(new Prism::Model2D)
	, myDefendMarker(new Prism::Model2D)
	, myDefendArrow(new Prism::Model2D)
	, myPowerUpPositions(8)
	, myConversation(" ")
	, myEnemiesTarget(nullptr)
	, myHealthBar(new Prism::Model2D)
	, myShieldBar(new Prism::Model2D)
	, myHealthBarGlow(new Prism::Model2D)
	, myShieldBarGlow(new Prism::Model2D)
	, myHitMarker(new Prism::Model2D)

{
	PostMaster::GetInstance()->Subscribe(eMessageType::RESIZE, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::CONVERSATION, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::DEFEND, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::BULLET_COLLISION_TO_GUI, this);
	CU::Vector2<float> arrowAndMarkerSize(64, 64);
	myReticle->Init("Data/Resource/Texture/UI/T_navigation_circle.dds", { 1024.f, 1024.f });
	mySteeringTarget->Init("Data/Resource/Texture/UI/T_crosshair_stearing.dds", arrowAndMarkerSize);
	myCrosshair->Init("Data/Resource/Texture/UI/T_crosshair_shooting.dds", { 256.f, 256.f }); // the size scales the pic
	myEnemyMarker->Init("Data/Resource/Texture/UI/T_navigation_marker_enemy.dds", arrowAndMarkerSize);
	myEnemyArrow->Init("Data/Resource/Texture/UI/T_navigation_arrow_enemy.dds", arrowAndMarkerSize);
	myWaypointArrow->Init("Data/Resource/Texture/UI/T_navigation_arrow_waypoint.dds", arrowAndMarkerSize);
	myWaypointMarker->Init("Data/Resource/Texture/UI/T_navigation_marker_waypoint.dds", arrowAndMarkerSize);
	myPowerUpArrow->Init("Data/Resource/Texture/UI/T_navigation_arrow_powerup.dds", arrowAndMarkerSize);
	myPowerUpMarker->Init("Data/Resource/Texture/UI/T_navigation_marker_powerup.dds", arrowAndMarkerSize);
	myDefendMarker->Init("Data/Resource/Texture/UI/T_defend_marker.dds", arrowAndMarkerSize);
	myDefendArrow->Init("Data/Resource/Texture/UI/T_defend_arrow.dds", arrowAndMarkerSize);

	myOriginalBarSize = 32.f;
	myBarSize = myOriginalBarSize;

	ReadXML();

	myHealthBarGlow->Init("Data/Resource/Texture/UI/T_health_bar_bar_a.dds", { myBarSize, myBarSize });
	myHealthBar->Init("Data/Resource/Texture/UI/T_health_bar_bar_b.dds", { myBarSize, myBarSize });

	myShieldBarGlow->Init("Data/Resource/Texture/UI/T_health_bar_bar_a.dds", { myBarSize, myBarSize });
	myShieldBar->Init("Data/Resource/Texture/UI/T_health_bar_bar_b.dds", { myBarSize, myBarSize });

	myHealthBarCount = 20;
	myShieldBarCount = 20;

	myHitMarker->Init("Data/Resource/Texture/UI/T_crosshair_shooting_hitmarks.dds", { 256, 256 });
}

GUIComponent::~GUIComponent()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::RESIZE, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::CONVERSATION, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::BULLET_COLLISION_TO_GUI, this);
	delete myReticle;
	delete mySteeringTarget;
	delete myCrosshair;
	delete myEnemyMarker;
	delete myEnemyArrow;
	delete myWaypointArrow;
	delete myWaypointMarker;
	delete myPowerUpArrow;
	delete myPowerUpMarker;
	delete myModel2DToRender;
	delete myDefendMarker;
	delete myDefendArrow;
	delete myHitMarker;
	myReticle = nullptr;
	myPowerUpArrow = nullptr;
	myPowerUpMarker = nullptr;
	myWaypointMarker = nullptr;
	myWaypointArrow = nullptr;
	mySteeringTarget = nullptr;
	myCrosshair = nullptr;
	myEnemyMarker = nullptr;
	myEnemyArrow = nullptr;
	myModel2DToRender = nullptr;
	myDefendArrow = nullptr;
	myDefendMarker = nullptr;
	myHitMarker = nullptr;

	delete myHealthBar;
	myHealthBar = nullptr;

}

void GUIComponent::Init(float aMaxDistanceToEnemies)
{
	myMaxDistanceToEnemies = aMaxDistanceToEnemies;
	myEnemiesTarget = &GetEntity();
}

void GUIComponent::Update(float aDeltaTime)
{
	myHitMarkerTimer -= aDeltaTime;

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
		aCurrentModel->Render(newRenderPos.x, newRenderPos.y);
	}
}

void GUIComponent::Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos)
{
	//Prism::Engine::GetInstance()->EnableAlphaBlending();

	Prism::Engine::GetInstance()->DisableZBuffer();
	float halfHeight = aWindowSize.y * 0.5f;
	float halfWidth = aWindowSize.x * 0.5f;
	CU::Vector2<float> steeringPos(halfWidth + mySteeringTargetPosition.x
		, -halfHeight - mySteeringTargetPosition.y);
	Prism::Engine::GetInstance()->PrintDebugText(myConversation, { halfWidth, -halfHeight - 200.f });
	myReticle->Render(halfWidth, -halfHeight);
	mySteeringTarget->Render(steeringPos.x, steeringPos.y);
	myCrosshair->Render(halfWidth, -(halfHeight));

	if (myEnemiesTarget != nullptr && myEnemiesTarget != &GetEntity())
	{
		Prism::Engine::GetInstance()->PrintDebugText("DefendTarget "
			+ myEnemiesTarget->GetComponent<PropComponent>()->GetDefendName() + ": "
			+ std::to_string(myEnemiesTarget->GetComponent<HealthComponent>()->GetHealth()) + " hp"
			, { halfWidth, -halfHeight });
	}

	CalculateAndRender(myWaypointPosition, myModel2DToRender, myWaypointArrow, myWaypointMarker
		, aWindowSize, myWaypointActive);

	for (int i = 0; i < myEnemiesPosition.Size(); ++i)
	{
		if (CU::Length(myEnemiesPosition[i] - myCamera->GetOrientation().GetPos()) < myMaxDistanceToEnemies)
		{
			CalculateAndRender(myEnemiesPosition[i], myModel2DToRender, myEnemyArrow, myEnemyMarker, aWindowSize, true);

		}
	}

	for (int i = 0; i < myPowerUpPositions.Size(); ++i)
	{
		CalculateAndRender(myPowerUpPositions[i], myModel2DToRender, myPowerUpArrow, myPowerUpMarker, aWindowSize, true);
	}

	if (myEnemiesTarget != nullptr)
	{
		CalculateAndRender(myEnemiesTarget->myOrientation.GetPos(), myModel2DToRender, myDefendArrow, myDefendMarker, aWindowSize, true);
	}

	myEnemiesPosition.RemoveAll();
	myPowerUpPositions.RemoveAll();

	for (int i = 0; i < myHealthBarCount; ++i)
	{
		CU::Vector2<float> newRenderPos = { 0.518f, -0.820f };

		newRenderPos += 1.f;
		newRenderPos *= 0.5f;
		newRenderPos.x *= aWindowSize.x;
		newRenderPos.y *= aWindowSize.y;
		newRenderPos.y -= aWindowSize.y;

		myHealthBarGlow->Render(newRenderPos.x + (i * 11.f), newRenderPos.y);
		myHealthBar->Render(newRenderPos.x + (i * 11.f), newRenderPos.y);
	}

	for (int i = 0; i < myShieldBarCount; ++i)
	{
		CU::Vector2<float> newRenderPos = { -0.739f, -0.812f };

		newRenderPos += 1.f;
		newRenderPos *= 0.5f;
		newRenderPos.x *= aWindowSize.x;
		newRenderPos.y *= aWindowSize.y;
		newRenderPos.y -= aWindowSize.y;

		myShieldBarGlow->Render(newRenderPos.x + (i * 11.f), newRenderPos.y);
		myShieldBar->Render(newRenderPos.x + (i * 11.f), newRenderPos.y);
	}

	if (myHitMarkerTimer >= 0.f)
	{
		myHitMarker->Render(steeringPos.x, steeringPos.y);
	}

	Prism::Engine::GetInstance()->EnableZBuffer();
	//Prism::Engine::GetInstance()->DisableAlpaBlending();
}

void GUIComponent::ReceiveNote(const MissionNote& aNote)
{
	if (aNote.myEvent == eMissionEvent::START && aNote.myType == eMissionType::WAYPOINT)
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

void GUIComponent::ReceiveNote(const HealthNote& aNote)
{
	myHealthBarCount = static_cast<int>(((aNote.myHealth / static_cast<float>(aNote.myMaxHealth) *
		20 + 0.5f)));
}

void GUIComponent::ReceiveNote(const ShieldNote& aNote)
{
	myShieldBarCount = static_cast<int>(((aNote.myShieldStrength / static_cast<float>(aNote.myMaxShieldStrength) *
		20 + 0.5f)));
}

void GUIComponent::ReceiveMessage(const ConversationMessage& aMessage)
{
	myConversation = aMessage.myText;
}

void GUIComponent::ReceiveMessage(const DefendMessage& aMessage)
{
	if (aMessage.myType == DefendMessage::eType::ENTITY)
	{
		myEnemiesTarget = aMessage.myEntity;
	}
	else if (aMessage.myType == DefendMessage::eType::COMPLETE)
	{
		myEnemiesTarget = nullptr;
	}
}

void GUIComponent::ReceiveMessage(const ResizeMessage& aMessage)
{
	float offset = aMessage.GetResolution().x / static_cast<float>(aMessage.GetResolution().y);

	myBarSize = (myOriginalBarSize * offset) * 0.5f;

	myHealthBar->SetSize({ myBarSize, myBarSize });
	myHealthBarGlow->SetSize({ myBarSize, myBarSize });

	myShieldBar->SetSize({ myBarSize, myBarSize });
	myShieldBarGlow->SetSize({ myBarSize, myBarSize });
}

void GUIComponent::ReceiveMessage(const BulletCollisionToGUIMessage& aMessage)
{
	if (aMessage.myBullet.GetType() == eEntityType::PLAYER_BULLET)
	{
		myHitMarkerTimer = 0.1f;
	}
}

void GUIComponent::ReadXML()
{
	XMLReader reader;
	reader.OpenDocument("Data/Resource/GUI/GUI_bar_health.xml");
	tinyxml2::XMLElement* root = reader.FindFirstChild("root");
	tinyxml2::XMLElement* position = reader.FindFirstChild(root, "Position");
	reader.ForceReadAttribute(position, "X", myOriginalHealthBarRenderPosition.x);
	reader.ForceReadAttribute(position, "Y", myOriginalHealthBarRenderPosition.y);

	reader.CloseDocument();

	reader.OpenDocument("Data/Resource/GUI/GUI_bar_shield.xml");
	root = reader.FindFirstChild("root");
	position = reader.FindFirstChild(root, "Position");
	reader.ForceReadAttribute(position, "X", myOriginalShieldBarRenderPosition.x);
	reader.ForceReadAttribute(position, "Y", myOriginalShieldBarRenderPosition.y);

	reader.CloseDocument();



	//myHealthBarRenderPosition = myOriginalHealthBarRenderPosition;
	//myShieldBarRenderPosition = myOriginalShieldBarRenderPosition;

	float offset = Prism::Engine::GetInstance()->GetWindowSize().y /
		static_cast<float>(Prism::Engine::GetInstance()->GetWindowSize().x);


	myBarSize = (myOriginalBarSize * offset);

}