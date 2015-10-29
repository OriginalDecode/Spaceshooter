#include "stdafx.h"
#include <Camera.h>
#include "BulletCollisionToGUIMessage.h"
#include "Constants.h"
#include "ConversationMessage.h"
#include "DefendMessage.h"
#include <Effect.h>
#include <EffectContainer.h>
#include <Engine.h>
#include <EngineEnums.h>
#include "Entity.h"
#include "Instance.h"
#include "GUIComponent.h"
#include "GUINote.h"
#include "HealthComponent.h"
#include "HealthNote.h"
#include "MissionNote.h"
#include "Model.h"
#include "ModelLoader.h"
#include "ModelProxy.h"
#include <Sprite.h>
#include "PostMaster.h"
#include "PowerUpComponent.h"
#include "PowerUpMessage.h"
#include "PropComponent.h"
#include "ResizeMessage.h"
#include "ShieldNote.h"
#include "ShootingComponent.h"
#include <sstream>
#include <XMLReader.h>

#define CIRCLERADIUS 400.f

GUIComponent::GUIComponent(Entity& aEntity)
	: Component(aEntity)
	, myWaypointActive(false)
	, myEnemies(16)
	, myCamera(nullptr)
	, myPowerUps(8)
	, myConversation(" ")
	, myEnemiesTarget(nullptr)
	, myHitMarkerTimer(-1.f)
	, myDamageIndicatorTimer(-1.f)
	, myClosestEnemy(nullptr)
	, myShowMessage(false)
	, myMessage("")
	, myMessageTime(0.f)
{
	PostMaster::GetInstance()->Subscribe(eMessageType::RESIZE, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::CONVERSATION, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::DEFEND, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::BULLET_COLLISION_TO_GUI, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::POWER_UP, this);



	XMLReader reader;
	reader.OpenDocument("Data/Resource/Model/Player/SM_cockpit_healthbar.xml");

	reader.ForceReadAttribute(reader.ForceFindFirstChild(reader.ForceFindFirstChild("root"), "radius")
		, "value", myHealthBarRadius);
	reader.CloseDocument();
	reader.OpenDocument("Data/Resource/Model/Player/SM_cockpit_shieldbar.xml");

	reader.ForceReadAttribute(reader.ForceFindFirstChild(reader.ForceFindFirstChild("root"), "radius")
		, "value", myShieldBarRadius);
	reader.CloseDocument();

	Prism::ModelProxy* model = Prism::Engine::GetInstance()->GetModelLoader()->LoadModel(
		"Data/Resource/Model/Player/SM_cockpit_healthbar.fbx", "Data/Resource/Shader/S_effect_bar_health.fx");
	myGUIBars[0] = new Prism::Instance(*model, myEntity.myOrientation, Prism::eOctreeType::DYNAMIC, myHealthBarRadius);

	Prism::ModelProxy* model2 = Prism::Engine::GetInstance()->GetModelLoader()->LoadModel(
		"Data/Resource/Model/Player/SM_cockpit_shieldbar.fbx", "Data/Resource/Shader/S_effect_bar_shield.fx");
	myGUIBars[1] = new Prism::Instance(*model2, myEntity.myOrientation, Prism::eOctreeType::DYNAMIC, myShieldBarRadius);


	Prism::Effect* hpBarEffect = Prism::Engine::GetInstance()->GetEffectContainer()->GetEffect(
		"Data/Resource/Shader/S_effect_bar_health.fx");
	hpBarEffect->SetPlayerVariable(1000);

	Prism::Effect* shieldBarEffect = Prism::Engine::GetInstance()->GetEffectContainer()->GetEffect(
		"Data/Resource/Shader/S_effect_bar_shield.fx");
	shieldBarEffect->SetPlayerVariable(1000);



	myReticle = new Prism::Sprite("Data/Resource/Texture/UI/T_navigation_circle.dds"
		, { 1024.f, 1024.f }, { 512.f, 512.f });
	CU::Vector2<float> arrowAndMarkerSize(64, 64);
	mySteeringTarget = new Prism::Sprite("Data/Resource/Texture/UI/T_crosshair_stearing.dds"
		, arrowAndMarkerSize, arrowAndMarkerSize / 2.f);
	myCrosshair = new Prism::Sprite("Data/Resource/Texture/UI/T_crosshair_shooting.dds"
		, { 256.f, 256.f }, { 128.f, 128.f }); // the size scales the pic
	myEnemyMarker = new Prism::Sprite("Data/Resource/Texture/UI/T_navigation_marker_enemy.dds"
		, arrowAndMarkerSize, arrowAndMarkerSize / 2.f);
	myEnemyArrow = new Prism::Sprite("Data/Resource/Texture/UI/T_navigation_arrow_enemy.dds"
		, arrowAndMarkerSize, arrowAndMarkerSize / 2.f);
	myWaypointArrow = new Prism::Sprite("Data/Resource/Texture/UI/T_navigation_arrow_waypoint.dds"
		, arrowAndMarkerSize, arrowAndMarkerSize / 2.f);
	myWaypointMarker = new Prism::Sprite("Data/Resource/Texture/UI/T_navigation_marker_waypoint.dds"
		, arrowAndMarkerSize, arrowAndMarkerSize / 2.f);
	myPowerUpArrow = new Prism::Sprite("Data/Resource/Texture/UI/T_navigation_arrow_powerup.dds"
		, arrowAndMarkerSize, arrowAndMarkerSize / 2.f);
	myPowerUpMarker = new Prism::Sprite("Data/Resource/Texture/UI/T_navigation_marker_powerup.dds"
		, arrowAndMarkerSize, arrowAndMarkerSize / 2.f);
	myDefendMarker = new Prism::Sprite("Data/Resource/Texture/UI/T_defend_marker.dds"
		, arrowAndMarkerSize, arrowAndMarkerSize / 2.f);
	myDefendArrow = new Prism::Sprite("Data/Resource/Texture/UI/T_defend_arrow.dds"
		, arrowAndMarkerSize, arrowAndMarkerSize / 2.f);

	myHitMarker = new Prism::Sprite("Data/Resource/Texture/UI/T_crosshair_shooting_hitmarks.dds"
		, { 256, 256 }, { 128.f, 128.f });

	CU::Vector2<float> screenSize = { float(Prism::Engine::GetInstance()->GetWindowSize().x),
		float(Prism::Engine::GetInstance()->GetWindowSize().y) };
	myDamageIndicator = new Prism::Sprite("Data/Resource/Texture/UI/T_damage_indicator.dds", screenSize, screenSize / 2.f);
	myHomingTarget = new Prism::Sprite("Data/Resource/Texture/UI/T_navigation_arrow_enemy.dds", { 100.f, 100.f }, { 50.f, 50.f });
}

GUIComponent::~GUIComponent()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::RESIZE, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::CONVERSATION, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::BULLET_COLLISION_TO_GUI, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::POWER_UP, this);
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
	delete myDamageIndicator;
	delete myHomingTarget;
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
	myDamageIndicator = nullptr;
	myHomingTarget = nullptr;

	delete myGUIBars[0];
	delete myGUIBars[1];

	myGUIBars[0] = nullptr;
	myGUIBars[1] = nullptr;
}

void GUIComponent::Init(float aMaxDistanceToEnemies)
{
	myMaxDistanceToEnemies = aMaxDistanceToEnemies;
	myEnemiesTarget = &GetEntity();
}

void GUIComponent::Update(float aDeltaTime)
{
	myHitMarkerTimer -= aDeltaTime;
	myDamageIndicatorTimer -= aDeltaTime;

	if (myShowMessage == true)
	{
		myMessageTime -= aDeltaTime;
		if (myMessageTime <= 0.f)
		{
			myShowMessage = false;
			myMessageTime = 0.f;
			myMessage = "";
		}
	}
}

void GUIComponent::CalculateAndRender(const CU::Vector3<float>& aPosition, Prism::Sprite* aCurrentModel
	, Prism::Sprite* aArrowModel, Prism::Sprite* aMarkerModel, const CU::Vector2<int> aWindowSize
	, bool aShowDist, bool aIsPowerup, std::string aName)
{
	bool showName = false;
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
		showName = true;
	}
	if (circleAroundPoint < 0.f)
	{
		aCurrentModel = aArrowModel;
		newRenderPos.x = -radius.x * CIRCLERADIUS + (halfWidth);
		newRenderPos.y = -(-radius.y * CIRCLERADIUS + (halfHeight));
		showName = false;
	}

	if (aIsPowerup == true && showName == true)
	{
		Prism::Engine::GetInstance()->PrintDebugText(aName, { newRenderPos.x - 16.f, newRenderPos.y + 64.f });
	}

	if (aShowDist == true)
	{
		if (aCurrentModel == myWaypointMarker || aCurrentModel == myWaypointArrow)
		{
			Prism::Engine::GetInstance()->PrintDebugText(lengthToWaypoint.str(), { newRenderPos.x - 16.f, newRenderPos.y + 64.f });
		}
		aCurrentModel->Render({ newRenderPos.x, newRenderPos.y });
		if (aArrowModel == myEnemyArrow)
		{
			myClosestScreenPos.x = newRenderPos.x;
			myClosestScreenPos.y = newRenderPos.y;
		}
	}
}

void GUIComponent::Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos)
{
	myClosestEnemyLength = 100000.f;
	myClosestEnemy = nullptr;

	Prism::Engine::GetInstance()->DisableZBuffer();
	float halfHeight = aWindowSize.y * 0.5f;
	float halfWidth = aWindowSize.x * 0.5f;
	CU::Vector2<float> steeringPos(halfWidth + mySteeringTargetPosition.x
		, -halfHeight - mySteeringTargetPosition.y);
	Prism::Engine::GetInstance()->PrintDebugText(myConversation, { halfWidth, -halfHeight - 200.f });
	myReticle->Render({ halfWidth, -halfHeight });
	mySteeringTarget->Render({ steeringPos.x, steeringPos.y });
	myCrosshair->Render({ halfWidth, -(halfHeight) });

	if (myEnemiesTarget != nullptr && myEnemiesTarget != &GetEntity())
	{
		Prism::Engine::GetInstance()->PrintDebugText("DefendTarget "
			+ myEnemiesTarget->GetComponent<PropComponent>()->GetDefendName() + ": "
			+ std::to_string(myEnemiesTarget->GetComponent<HealthComponent>()->GetHealth()) + " hp"
			, { halfWidth, -halfHeight });
	}

	CalculateAndRender(myWaypointPosition, myModel2DToRender, myWaypointArrow, myWaypointMarker
		, aWindowSize, myWaypointActive);

	for (int i = 0; i < myEnemies.Size(); ++i)
	{
		float lengthToEnemy = CU::Length(myEnemies[i]->myOrientation.GetPos() - myCamera->GetOrientation().GetPos());
		if (lengthToEnemy < myMaxDistanceToEnemies)
		{
			CalculateAndRender(myEnemies[i]->myOrientation.GetPos(), myModel2DToRender, myEnemyArrow, myEnemyMarker, aWindowSize, true);
			CU::Vector2<float> enemyScreenPos = myClosestScreenPos;
			float lengthFromMouseToEnemy = CU::Length(enemyScreenPos - CU::Vector2<float>(steeringPos.x, steeringPos.y));
			if (lengthFromMouseToEnemy < myClosestEnemyLength)
			{
				myClosestEnemy = myEnemies[i];
				myClosestEnemyLength = lengthFromMouseToEnemy;
			}
		}
	}

	for (int i = 0; i < myPowerUps.Size(); ++i)
	{
		CalculateAndRender(myPowerUps[i]->myOrientation.GetPos(), myModel2DToRender, myPowerUpArrow, myPowerUpMarker
			, aWindowSize, true, true, myPowerUps[i]->GetComponent<PowerUpComponent>()->GetInGameName());
	}

	if (myEnemiesTarget != nullptr)
	{
		CalculateAndRender(myEnemiesTarget->myOrientation.GetPos(), myModel2DToRender, myDefendArrow, myDefendMarker, aWindowSize, true);
	}

	if (myHasHomingWeapon == true)
	{
		if (myClosestEnemy != nullptr)
		{
			CalculateAndRender(myClosestEnemy->myOrientation.GetPos(), myModel2DToRender, myHomingTarget, myHomingTarget, aWindowSize, true);
		}
		myEntity.GetComponent<ShootingComponent>()->SetHomingTarget(myClosestEnemy);
	}

	myEnemies.RemoveAll();
	myPowerUps.RemoveAll();

	myGUIBars[0]->Render(*myCamera);
	myGUIBars[1]->Render(*myCamera);

	if (myHitMarkerTimer >= 0.f)
	{
		myHitMarker->Render({ steeringPos.x, steeringPos.y });
	}

	if (myDamageIndicatorTimer >= 0.f)
	{
		myDamageIndicator->Render({ halfWidth, -halfHeight });
	}

	if (myShowMessage == true)
	{
		Prism::Engine::GetInstance()->PrintDebugText(myMessage, { 100.f, -100.f });
	}

	Prism::Engine::GetInstance()->EnableZBuffer();
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
		myWaypointPosition = aNote.myEntity->myOrientation.GetPos();
		break;
	case eGUINoteType::ENEMY:
		myEnemies.Add(aNote.myEntity);
		break;
	case eGUINoteType::POWERUP:
		myPowerUps.Add(aNote.myEntity);
		break;
	case eGUINoteType::STEERING_TARGET:
		mySteeringTargetPosition = { aNote.myPosition.x, aNote.myPosition.y };
		break;
	case eGUINoteType::HOMING_TARGET:
		myHasHomingWeapon = aNote.myIsHoming;
		break;
	default:
		break;
	}
}

void GUIComponent::ReceiveNote(const HealthNote& aNote)
{
	Prism::Effect* hpBarEffect = Prism::Engine::GetInstance()->GetEffectContainer()->GetEffect(
		"Data/Resource/Shader/S_effect_bar_health.fx");
	hpBarEffect->SetPlayerVariable(aNote.myHealth);
}

void GUIComponent::ReceiveNote(const ShieldNote& aNote)
{
	Prism::Effect* shieldBarEffect = Prism::Engine::GetInstance()->GetEffectContainer()->GetEffect(
		"Data/Resource/Shader/S_effect_bar_shield.fx");
	shieldBarEffect->SetPlayerVariable(aNote.myShieldStrength);
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
}

void GUIComponent::ReceiveMessage(const BulletCollisionToGUIMessage& aMessage)
{
	if (aMessage.myBullet.GetType() == eEntityType::PLAYER_BULLET)
	{
		myHitMarkerTimer = 0.1f;
	}
	else if (aMessage.myBullet.GetType() == eEntityType::ENEMY_BULLET && &aMessage.myEntityCollidedWith == &GetEntity())
	{
		myDamageIndicatorTimer = 0.1f;

		myCamera->ShakeCamera(100.f, 6.f, 0.5f); // pilla inte på dessa siffror!
	}
}

void GUIComponent::ReceiveMessage(const PowerUpMessage& aMessage)
{
	myMessage = "Weapon upgrade received: " + aMessage.GetUprgade();
	myMessageTime = 5.f;
	myShowMessage = true;
}

void GUIComponent::Reset()
{

	Prism::Effect* shieldBarEffect = Prism::Engine::GetInstance()->GetEffectContainer()->GetEffect(
		"Data/Resource/Shader/S_effect_bar_shield.fx");
	shieldBarEffect->SetPlayerVariable(1000);
	
	Prism::Effect* hpBarEffect = Prism::Engine::GetInstance()->GetEffectContainer()->GetEffect(
		"Data/Resource/Shader/S_effect_bar_health.fx");
	hpBarEffect->SetPlayerVariable(1000);

	myEnemiesTarget = nullptr;
}