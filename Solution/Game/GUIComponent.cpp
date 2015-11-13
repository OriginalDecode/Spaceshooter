#include "stdafx.h"
#include "AudioInterface.h"
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
#include "InputNote.h"
#include "Instance.h"
#include "GUIComponent.h"
#include "GUINote.h"
#include "HealthComponent.h"
#include "HealthNote.h"
#include "KillStructureMessage.h"
#include <MathHelper.h>
#include "MissionNote.h"
#include "Model.h"
#include "ModelLoader.h"
#include "ModelProxy.h"
#include <Sprite.h>
#include "PhysicsComponent.h"
#include "PostMaster.h"
#include "PowerUpComponent.h"
#include "PowerUpGUIIcon.h"
#include "PowerUpMessage.h"
#include "PowerUpNote.h"
#include "PropComponent.h"
#include "ResizeMessage.h"
#include "ShieldNote.h"
#include "ShootingComponent.h"
#include "SoundNote.h"
#include <sstream>
#include <XMLReader.h>

#define CIRCLERADIUS 400.f

GUIComponent::GUIComponent(Entity& aEntity)
	: Component(aEntity)
	, myWaypointActive(false)
	, myEnemies(64)
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
	, myWeapon("")
	, my3DClosestEnemyLength(10000)
	, myBattlePlayed(false)
	, myBackgroundMusicPlayed(true)
	, myDeltaTime(0.f)
	, myHasRocketLauncher(false)
	, myHasShotgun(false)
	, myHasMachinegun(false)
	, myRocketCurrentTime(nullptr)
	, myRocketMaxTime(nullptr)
	, myCurrentHitmarker(nullptr)
	, myCurrentShield(100.f)
	, myPlayedMissilesReady(false)
	, myCockpitOffset(CalcCockpitOffset())
	, myShowTutorialMessage(false)
	, myTutorialMessage("")
{
	PostMaster::GetInstance()->Subscribe(eMessageType::RESIZE, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::CONVERSATION, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::DEFEND, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::BULLET_COLLISION_TO_GUI, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::POWER_UP, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::KILL_STRUCTURE, this);

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
	myGUIBars[0] = new Prism::Instance(*model, *GetCockpitOrientation(), Prism::eOctreeType::DYNAMIC, myHealthBarRadius);

	Prism::ModelProxy* model2 = Prism::Engine::GetInstance()->GetModelLoader()->LoadModel(
		"Data/Resource/Model/Player/SM_cockpit_shieldbar.fbx", "Data/Resource/Shader/S_effect_bar_shield.fx");
	myGUIBars[1] = new Prism::Instance(*model2, *GetCockpitOrientation(), Prism::eOctreeType::DYNAMIC, myShieldBarRadius);

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
		, { 256.f, 256.f }, { 128.f, 128.f });
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

	myDefendHitMarker = new Prism::Sprite("Data/Resource/Texture/UI/T_crosshair_shooting_hitmarks_defend.dds"
		, { 256, 256 }, { 128.f, 128.f });

	myPropHitMarker = new Prism::Sprite("Data/Resource/Texture/UI/T_crosshair_shooting_hitmarks_prop.dds"
		, { 256, 256 }, { 128.f, 128.f });

	CU::Vector2<float> screenSize = { float(Prism::Engine::GetInstance()->GetWindowSize().x),
		float(Prism::Engine::GetInstance()->GetWindowSize().y) };
	myDamageIndicatorHealth = new Prism::Sprite("Data/Resource/Texture/UI/T_damage_indicator_health.dds", screenSize, screenSize / 2.f);
	myDamageIndicatorShield = new Prism::Sprite("Data/Resource/Texture/UI/T_damage_indicator_shield.dds", screenSize, screenSize / 2.f);
	myHomingTarget = new Prism::Sprite("Data/Resource/Texture/UI/T_navigation_marker_enemy_lock.dds", { 100.f, 100.f }, { 50.f, 50.f });

	myStructureMarker = new Prism::Sprite("Data/Resource/Texture/UI/T_navigation_marker_structure.dds"
		, arrowAndMarkerSize, arrowAndMarkerSize / 2.f);
	myStructureArrow = new Prism::Sprite("Data/Resource/Texture/UI/T_navigation_arrow_structure.dds"
		, arrowAndMarkerSize, arrowAndMarkerSize / 2.f);

	myBackgroundConversation = new Prism::Sprite("Data/Resource/Texture/UI/T_background_conversation.dds"
		, { 1024.f, 256.f }, { 0, 0 });
	myBackgroundMission = new Prism::Sprite("Data/Resource/Texture/UI/T_background_mission.dds"
		, { 512.f, 512.f }, { 0, 0 });
}

GUIComponent::~GUIComponent()
{
	PostMaster::GetInstance()->UnSubscribe(eMessageType::RESIZE, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::CONVERSATION, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::BULLET_COLLISION_TO_GUI, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::POWER_UP, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::KILL_STRUCTURE, this);
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
	delete myDamageIndicatorHealth;
	delete myDamageIndicatorShield;
	delete myHomingTarget;
	delete myPropHitMarker;
	//delete myCurrentHitmarker; // dont delete, please!
	delete myDefendHitMarker;
	delete myBackgroundConversation;
	delete myBackgroundMission;
	myPropHitMarker = nullptr;
	myCurrentHitmarker = nullptr;
	myDefendHitMarker = nullptr;
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
	myDamageIndicatorHealth = nullptr;
	myDamageIndicatorShield = nullptr;
	myHomingTarget = nullptr;
	myBackgroundConversation = nullptr;
	myBackgroundMission = nullptr;

	delete myGUIBars[0];
	delete myGUIBars[1];

	myGUIBars[0] = nullptr;
	myGUIBars[1] = nullptr;

	delete myPowerUpSlots[ePowerUpType::EMP];
	delete myPowerUpSlots[ePowerUpType::FIRERATEBOOST];
	delete myPowerUpSlots[ePowerUpType::HOMING];
	delete myPowerUpSlots[ePowerUpType::INVULNERABLITY];
}

void GUIComponent::Init(float aMaxDistanceToEnemies)
{
	myMaxDistanceToEnemies = aMaxDistanceToEnemies;
	myEnemiesTarget = &GetEntity();

	CU::Vector2<float> halfScreenSize = { float(Prism::Engine::GetInstance()->GetWindowSize().x * 0.5f),
		float(Prism::Engine::GetInstance()->GetWindowSize().y * 0.5f) };

	CU::Vector2<float> iconSize(64.f, 64.f);
	float padding = 5.f;

	ShootingComponent* shootingComponent = myEntity.GetComponent<ShootingComponent>();

	myCurrentWeapon = &shootingComponent->GetCurrentWeaponID();


	myPowerUpSlots[ePowerUpType::EMP] = new PowerUpGUIIcon("Data/Resource/Texture/UI/PowerUp/T_powerup_emp_active.dds"
		, "Data/Resource/Texture/UI/PowerUp/T_powerup_emp_inactive.dds", { iconSize.x * 6.75f, iconSize.y * 2.5f + padding * 2.f }
	, shootingComponent->GetEMPPowerUp(), nullptr);

	myPowerUpSlots[ePowerUpType::FIRERATEBOOST] = new PowerUpGUIIcon("Data/Resource/Texture/UI/PowerUp/T_powerup_firerate_active.dds"
		, "Data/Resource/Texture/UI/PowerUp/T_powerup_firerate_inactive.dds", { iconSize.x * 7.1f, iconSize.y * 1.5f + padding * 1.f }
	, shootingComponent->GetFireRatePowerUp(), &shootingComponent->GetFireRatePowerUpDuration());

	myPowerUpSlots[ePowerUpType::HOMING] = new PowerUpGUIIcon("Data/Resource/Texture/UI/PowerUp/T_powerup_homing_active.dds"
		, "Data/Resource/Texture/UI/PowerUp/T_powerup_homing_inactive.dds", { iconSize.x * 7.1f, iconSize.y * -1.5f + padding * 0.f }
	, shootingComponent->GetHomingPowerUp(), &shootingComponent->GetHomingPowerUpDuration());

	myPowerUpSlots[ePowerUpType::INVULNERABLITY] = new PowerUpGUIIcon("Data/Resource/Texture/UI/PowerUp/T_powerup_invulnerable_active.dds"
		, "Data/Resource/Texture/UI/PowerUp/T_powerup_invulnerable_inactive.dds", { iconSize.x * 6.75f, iconSize.y * -2.5f + padding * -1.f }
	, myEntity.GetComponent<HealthComponent>()->GetInvulnerability(), &myEntity.GetComponent<HealthComponent>()->GetInvulnerablityDuration());
}

void GUIComponent::Update(float aDeltaTime)
{
	myHitMarkerTimer -= aDeltaTime;
	myDamageIndicatorTimer -= aDeltaTime;
	myDeltaTime = aDeltaTime;

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
	, Prism::Sprite* aArrowModel, Prism::Sprite* aMarkerModel, const CU::Vector2<int>& aWindowSize
	, bool aShowDist, float anAlpha, bool aIsPowerup, std::string aName)
{
	bool showName = false;
	float halfWidth = aWindowSize.x *0.5f;
	float halfHeight = aWindowSize.y * 0.5f;

	CU::Vector3<float> toTarget = aPosition - myCamera->GetOrientation().GetPos();
	float lengthToTarget = CU::Length(toTarget);
	std::stringstream lengthToWaypoint;
	if (aShowDist == true)
	{
		lengthToWaypoint << static_cast<int>(CU::Length(toTarget) - 150);
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
		if (aArrowModel == myHomingTarget)
		{
			myClosestEnemy = nullptr;
			return;
		}
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
		if (aArrowModel == myHomingTarget)
		{
			myClosestEnemy = nullptr;
			return;
		}
	}

	if (aIsPowerup == true && showName == true)
	{
		Prism::Engine::GetInstance()->PrintText(aName, { newRenderPos.x - 46.f, newRenderPos.y + 40.f }, Prism::eTextType::RELEASE_TEXT);
	}

	if (aShowDist == true)
	{
		float scale = 1.f;
		if (aCurrentModel == myPowerUpArrow || aCurrentModel == myEnemyArrow || aCurrentModel == myDefendArrow 
			|| aCurrentModel == myWaypointArrow || aCurrentModel == myStructureArrow)
		{
			if (lengthToTarget <= 100)
			{
				anAlpha = 1.f;
				scale = 1.f;
			}
			else if (lengthToTarget >= myMaxDistanceToEnemies * 0.1)
			{
				anAlpha = 0.5f;
				scale = 0.5f;
			}
			else
			{
				anAlpha = CU::Math::Remap<float>(lengthToTarget, 100, myMaxDistanceToEnemies * 0.1f, 1.f, 0.5f);
				scale = CU::Math::Remap<float>(lengthToTarget, 100, myMaxDistanceToEnemies * 0.1f, 1.f, 0.5f);
			}
		}

		if (length < CIRCLERADIUS && circleAroundPoint > 0.f && aCurrentModel == myWaypointMarker
			|| length < CIRCLERADIUS && circleAroundPoint > 0.f && aCurrentModel == myWaypointArrow)
		{
			Prism::Engine::GetInstance()->PrintText(lengthToWaypoint.str(), { newRenderPos.x - 20.f, newRenderPos.y + 40.f }, Prism::eTextType::RELEASE_TEXT);
		}
		aCurrentModel->Render({ newRenderPos.x, newRenderPos.y }, { scale, scale }, { 1.f, 1.f, 1.f, anAlpha });
		if (aArrowModel == myEnemyArrow || aArrowModel == myStructureArrow)
		{
			myClosestScreenPos.x = newRenderPos.x;
			myClosestScreenPos.y = newRenderPos.y;
		}
	}
}

void GUIComponent::Render(const CU::Vector2<int>& aWindowSize, const CU::Vector2<float>& aMousePos)
{
	aMousePos;
	my3DClosestEnemyLength = 10000.f;
	myClosestEnemyLength = 100000.f;
	myClosestEnemy = nullptr;

	Prism::Engine::GetInstance()->DisableZBuffer();
	float halfHeight = aWindowSize.y * 0.5f;
	float halfWidth = aWindowSize.x * 0.5f;
	CU::Vector2<float> steeringPos(halfWidth + mySteeringTargetPosition.x
		, -halfHeight - mySteeringTargetPosition.y);
	if (myConversation.size() > 1)
	{
		myBackgroundConversation->Render({ 0, 0 });
	}
	myBackgroundMission->Render({ halfWidth * 0.15f - 128.f, -halfHeight * 1.2f + 20.f + 128.f });

	Prism::Engine::GetInstance()->PrintText(myConversation, { 50.f, -50.f }, Prism::eTextType::RELEASE_TEXT);

	CU::Vector2<float> crosshairPosition(CU::Math::Lerp<CU::Vector2<float>>({ halfWidth, -halfHeight }
		, { steeringPos.x, steeringPos.y }, 0.3f));

	myReticle->Render({ halfWidth, -halfHeight });
	mySteeringTarget->Render({ steeringPos.x, steeringPos.y });
	myCrosshair->Render(crosshairPosition);

	CalculateAndRender(myWaypointPosition, myModel2DToRender, myWaypointArrow, myWaypointMarker
		, aWindowSize, myWaypointActive);

	for (int i = 0; i < myEnemies.Size(); ++i)
	{
		float lengthToEnemy = CU::Length(myEnemies[i]->myOrientation.GetPos() - myCamera->GetOrientation().GetPos());
		if (lengthToEnemy < my3DClosestEnemyLength)
		{
			my3DClosestEnemyLength = lengthToEnemy;
		}

		if (myEnemies[i]->GetType() == eEntityType::STRUCTURE)
		{
			CalculateAndRender(myEnemies[i]->myOrientation.GetPos(), myModel2DToRender, myStructureArrow, myStructureMarker, aWindowSize, true);
		}
		else
		{
			if (lengthToEnemy < myMaxDistanceToEnemies)
			{
				CalculateAndRender(myEnemies[i]->myOrientation.GetPos(), myModel2DToRender, myEnemyArrow, myEnemyMarker, aWindowSize, true);
			}
		}
		CU::Vector2<float> enemyScreenPos = myClosestScreenPos;
		float lengthFromMouseToEnemy = CU::Length(enemyScreenPos - CU::Vector2<float>(steeringPos.x, steeringPos.y));
		if (lengthFromMouseToEnemy < myClosestEnemyLength)
		{
			myClosestEnemy = myEnemies[i];
			myClosestEnemyLength = lengthFromMouseToEnemy;
		}
	}

	if (myEnemies.Size() > 0 && my3DClosestEnemyLength < 1000)
	{
		if (myBattlePlayed == false)
		{
			Prism::Audio::AudioInterface::GetInstance()->PostEvent("Pause_BackgroundMusic", 0);
			Prism::Audio::AudioInterface::GetInstance()->PostEvent("Resume_BattleMusic", 0);

		}
		myBackgroundMusicPlayed = false;
		myBattlePlayed = true;
	}
	else
	{
		if (myBackgroundMusicPlayed == false)
		{
			Prism::Audio::AudioInterface::GetInstance()->PostEvent("Resume_BackgroundMusic", 0);
			Prism::Audio::AudioInterface::GetInstance()->PostEvent("Pause_BattleMusic", 0);
		}
		myBattlePlayed = false;
		myBackgroundMusicPlayed = true;
	}

	for (int i = 0; i < myPowerUps.Size(); ++i)
	{
		CalculateAndRender(myPowerUps[i]->myOrientation.GetPos(), myModel2DToRender, myPowerUpArrow, myPowerUpMarker
			, aWindowSize, true, true, 1.f, myPowerUps[i]->GetComponent<PowerUpComponent>()->GetInGameName());
	}

	if (myEnemiesTarget != nullptr)
	{
		CalculateAndRender(myEnemiesTarget->myOrientation.GetPos(), myModel2DToRender, myDefendArrow, myDefendMarker, aWindowSize, true);
	}

	float percentageToReady = 1.f;

	if (myHasRocketLauncher == true)
	{
		Prism::Engine::GetInstance()->PrintText("RL", { halfWidth + 550.f, -halfHeight + 15.f }, Prism::eTextType::RELEASE_TEXT);

		percentageToReady = *myRocketCurrentTime / *myRocketMaxTime;
		if (percentageToReady >= 1.f)
		{
			Prism::Engine::GetInstance()->PrintText("RDY", { halfWidth + 550.f, -halfHeight - 20.f }, Prism::eTextType::RELEASE_TEXT);
		}
	}
	else
	{
		myPlayedMissilesReady = false;
	}


	if (myHasHomingWeapon == true)
	{
		if (myClosestEnemy != nullptr)
		{
			myHomingTarget->Rotate(myDeltaTime);
			CalculateAndRender(myClosestEnemy->myOrientation.GetPos(), myModel2DToRender, myHomingTarget, myHomingTarget, aWindowSize, true, 1.f);
		}
		myEntity.GetComponent<ShootingComponent>()->SetHomingTarget(myClosestEnemy);
	}
	else if (myHasRocketLauncher == true && percentageToReady >= 1.f)
	{
		if (myClosestEnemy != nullptr)
		{
			myHomingTarget->Rotate(myDeltaTime);
			CalculateAndRender(myClosestEnemy->myOrientation.GetPos(), myModel2DToRender, myHomingTarget, myHomingTarget, aWindowSize, true, percentageToReady);
		}

		myEntity.GetComponent<ShootingComponent>()->SetHomingTarget(myClosestEnemy);
	}

	myEnemies.RemoveAll();
	myPowerUps.RemoveAll();

	myGUIBars[0]->Render(*myCamera);
	myGUIBars[1]->Render(*myCamera);

	if (myHitMarkerTimer >= 0.f)
	{
		myCurrentHitmarker->Render(crosshairPosition);
	}

	if (myDamageIndicatorTimer >= 0.f)
	{
		if (myCurrentShield <= 0)
		{
			myDamageIndicatorHealth->Render({ halfWidth, -halfHeight });
		}
		else
		{
			myDamageIndicatorShield->Render({ halfWidth, -halfHeight });
		}
	}

	if (myShowMessage == true)
	{
		Prism::Engine::GetInstance()->PrintText(myMessage, { halfWidth - 150.f, -halfHeight + 200.f }, Prism::eTextType::RELEASE_TEXT);
	}

	if (myShowTutorialMessage == true)
	{
		Prism::Engine::GetInstance()->PrintText(myTutorialMessage, { halfWidth - 130.f, -halfHeight + 220.f }, Prism::eTextType::RELEASE_TEXT);
	}

	myPowerUpSlots[ePowerUpType::EMP]->Render(aWindowSize);
	myPowerUpSlots[ePowerUpType::FIRERATEBOOST]->Render(aWindowSize);
	myPowerUpSlots[ePowerUpType::HOMING]->Render(aWindowSize);
	myPowerUpSlots[ePowerUpType::INVULNERABLITY]->Render(aWindowSize);

	if (myHasMachinegun == true)
	{
		Prism::Engine::GetInstance()->PrintText("MG", { halfWidth + 420.f, -halfHeight + 15.f }, Prism::eTextType::RELEASE_TEXT);

		if (*myCurrentWeapon == 0)
		{
			Prism::Engine::GetInstance()->PrintText("RDY", { halfWidth + 420.f, -halfHeight - 20.f }, Prism::eTextType::RELEASE_TEXT);
		}
	}

	if (myHasShotgun == true)
	{
		Prism::Engine::GetInstance()->PrintText("SG", { halfWidth + 480.f, -halfHeight + 15.f }, Prism::eTextType::RELEASE_TEXT);

		if (*myCurrentWeapon == 1)
		{
			Prism::Engine::GetInstance()->PrintText("RDY", { halfWidth + 480.f, -halfHeight - 20.f }, Prism::eTextType::RELEASE_TEXT);
		}
	}


	Prism::Engine::GetInstance()->PrintText(int(myEntity.GetComponent<PhysicsComponent>()->GetSpeed())
		, { 600.f, -800.f }, Prism::eTextType::RELEASE_TEXT);

	if (myEntity.GetComponent<ShootingComponent>()->HasPowerUp(ePowerUpType::EMP) == true)
	{
		Prism::Engine::GetInstance()->PrintText("EMP ready. Press [Space] to release."
			, { 100.f, -400.f }, Prism::eTextType::RELEASE_TEXT);
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
	shieldBarEffect->SetPlayerVariable(int(aNote.myShieldStrength));
	myCurrentShield = aNote.myShieldStrength;
}

void GUIComponent::ReceiveNote(const PowerUpNote& aNote)
{
	myMessage = aNote.myIngameName;
	myMessageTime = 3.f;
	myShowMessage = true;
}

void GUIComponent::ReceiveNote(const InputNote& aMessage)
{
	if (aMessage.myKey == 0 && myEntity.GetComponent<ShootingComponent>()->GetWeaponSize() > 0)
	{
		myWeapon = "MG\nRDY";
	}
	else if (aMessage.myKey == 1 && myEntity.GetComponent<ShootingComponent>()->GetWeaponSize() > 1)
	{
		myWeapon = "SG\nRDY";
	}
}

void GUIComponent::ReceiveMessage(const ConversationMessage& aMessage)
{
	myConversation = aMessage.myText;
	Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_ConvoInc", 0);
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
	aMessage;
	myCockpitOffset = CalcCockpitOffset();
}

void GUIComponent::ReceiveMessage(const BulletCollisionToGUIMessage& aMessage)
{
	if (aMessage.myBulletOwner == eEntityType::PLAYER_BULLET)
	{
		myHitMarkerTimer = 0.1f;
		if (aMessage.myEntityCollidedWith.GetType() == eEntityType::ENEMY
			|| aMessage.myEntityCollidedWith.GetType() == eEntityType::STRUCTURE)
		{
			myCurrentHitmarker = myHitMarker;
		}
		else if (aMessage.myEntityCollidedWith.GetType() == eEntityType::DEFENDABLE)
		{
			myCurrentHitmarker = myDefendHitMarker;
		}
		else if (aMessage.myEntityCollidedWith.GetType() == eEntityType::PROP)
		{
			myCurrentHitmarker = myPropHitMarker;
		}
	}
	else if (aMessage.myBullet.GetType() == eEntityType::ENEMY_BULLET && &aMessage.myEntityCollidedWith == &GetEntity())
	{
		myDamageIndicatorTimer = 0.1f;
		myEntity.SendNote<SoundNote>(SoundNote(eSoundNoteType::PLAY, "Play_PlayerHit"));
		//Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_PlayerHit", 0);
		myCamera->ShakeCamera(100.f, 6.f, 0.5f); // pilla inte p� dessa siffror!
	}
}

void GUIComponent::ReceiveMessage(const PowerUpMessage& aMessage)
{
	myMessage = aMessage.GetPickupMessage();
	myMessageTime = aMessage.GetPickupMessageTime();
	myShowMessage = true;

	if (aMessage.GetUpgradeID() == 0)
	{
		myHasMachinegun = true;
	}
	else if (aMessage.GetUpgradeID() == 1)
	{
		myHasShotgun = true;
	}
}

void GUIComponent::ReceiveMessage(const KillStructureMessage& aMessage)
{
	if (aMessage.myType == KillStructureMessage::eType::TO_GUI)
	{
		myEnemies.Add(aMessage.myEntity);
	}

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
	myClosestEnemy = nullptr;
	myHasRocketLauncher = false;
	myWaypointActive = false;

	ShootingComponent* shootingComp = myEntity.GetComponent<ShootingComponent>();
	int weaponSize = shootingComp->GetWeaponSize();
	if (weaponSize > 0)
	{
		int weaponID = shootingComp->GetCurrentWeaponID();
		if (weaponID == 0)
		{
			myHasMachinegun = true;
		}
		else if (weaponID == 1)
		{
			myHasShotgun = true;
		}
		if (weaponSize >= 2)
		{
			myHasRocketLauncher = true;
			myRocketCurrentTime = &shootingComp->GetRocketCurrentTime();
			myRocketMaxTime = &shootingComp->GetRocketMaxTime();
		}
	}
	else
	{
		myWeapon = "";
	}




	myConversation = " ";
	myEnemiesTarget = nullptr;
	myHitMarkerTimer = -1.f;
	myDamageIndicatorTimer = -1.f;
	myClosestEnemy = nullptr;
	myShowMessage = false;
	myMessage = "";
	myMessageTime = 0.0f;
	myWeapon = "";
	my3DClosestEnemyLength = 10000;
	myBattlePlayed = false;
	myBackgroundMusicPlayed = true;
	myDeltaTime = 0.f;
}

CU::Vector3<float> GUIComponent::CalcCockpitOffset() const
{
	CU::Vector2<float> resolution(float(Prism::Engine::GetInstance()->GetWindowSize().x), float(Prism::Engine::GetInstance()->GetWindowSize().y));

	float aspect = resolution.x / resolution.y;
	float epsilon = 0.05f;
	if (aspect < epsilon + 1280.f / 1024.f)
	{
		return CU::Vector3<float>(0, 0, -0.2f);
	}
	else if (aspect < epsilon + 1920.f / 1200.f)
	{
		return CU::Vector3<float>(0, 0, -0.1f);
	}

	return CU::Vector3<float>(0, 0, 0);
}

void GUIComponent::UpdateWeapons()
{
	int weaponSize = myEntity.GetComponent<ShootingComponent>()->GetWeaponSize();

	myHasMachinegun = weaponSize >= 1 ? true : false;
	myHasShotgun = weaponSize >= 2 ? true : false;
	myHasRocketLauncher = weaponSize >= 3 ? true : false;
}

void GUIComponent::ShowTutorialMessage(const std::string& aMessage)
{
	myShowTutorialMessage = true;
	myTutorialMessage = aMessage;
}

void GUIComponent::RemoveTutorialMessage()
{
	myShowTutorialMessage = false;
	myTutorialMessage = "";
}