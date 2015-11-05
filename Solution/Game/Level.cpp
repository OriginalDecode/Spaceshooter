#include "stdafx.h"

#include "AIComponent.h"
#include <AudioInterface.h>
#include "BulletManager.h"
#include <Camera.h>
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "Constants.h"
#include "ConversationManager.h"
#include "DefendMessage.h"
#include "EffectContainer.h"
#include "EMPMessage.h"
#include <Engine.h>
#include <EngineEnums.h>
#include "Entity.h"
#include "EntityFactory.h"
#include "ParticleEmitterComponent.h"
#include "EmitterManager.h"
#include "EventManager.h"
#include <FileWatcher.h>
#include "GameStateMessage.h"
#include "GraphicsComponent.h"
#include "GUIComponent.h"
#include "GUINote.h"
#include "HealthComponent.h"
#include "Instance.h"
#include "InputComponent.h"
#include <InputWrapper.h>
#include "Level.h"
#include "MissionManager.h"
#include "ModelLoader.h"
#include "ModelProxy.h"
#include "PhysicsComponent.h"
#include "PostMaster.h"
#include "PowerUpComponent.h"
#include "PowerUpMessage.h"
#include "PropComponent.h"
#include <Renderer.h>
#include <Scene.h>
#include "ShieldComponent.h"
#include "ShootingComponent.h"
#include "SpawnEnemyMessage.h"
#include "SpawnPowerUpMessage.h"
#include "StreakEmitterComponent.h"
#include "WeaponFactory.h"
#include <XMLReader.h>

Level::Level(CU::InputWrapper* aInputWrapper)
	: myEntities(16)
	, myComplete(false)
	, myUsePostProcessing(true)
	, mySkySphere(nullptr)
	, myEntityFactory(nullptr)
	, myWeaponFactory(nullptr)
	, myBulletManager(nullptr)
	, myCollisionManager(nullptr)
	, myMissionManager(nullptr)
	, myEventManager(nullptr)
	, myConversationManager(nullptr)
	, myEntityToDefend(nullptr)
	, myEmitterManager(nullptr)
	, myEMP(nullptr)
	, myEMPScale(1.f)
	, myEMPTimer(0.f)
	, myEMPActivated(false)
{
	myInputWrapper = aInputWrapper;
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_ENEMY, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::POWER_UP, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::DEFEND, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::SPAWN_POWERUP, this);
	PostMaster::GetInstance()->Subscribe(eMessageType::EMP, this);
	Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_BackgroundMusic", 0);
	Prism::Audio::AudioInterface::GetInstance()->PostEvent("Play_BattleMusic", 0);
	Prism::Audio::AudioInterface::GetInstance()->PostEvent("Pause_BattleMusicNoFade", 0);

	myRenderer = new Prism::Renderer();
}

Level::~Level()
{
	Prism::Engine::GetInstance()->GetModelLoader()->ClearLoadJobs();
	while (Prism::Engine::GetInstance()->GetModelLoader()->IsLoading() == true)
	{
	}
	Prism::Audio::AudioInterface::GetInstance()->PostEvent("Stop_BackgroundMusic", 0);
	Prism::Audio::AudioInterface::GetInstance()->PostEvent("Stop_BattleMusic", 0);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_ENEMY, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::POWER_UP, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::DEFEND, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::SPAWN_POWERUP, this);
	PostMaster::GetInstance()->UnSubscribe(eMessageType::EMP, this);
	delete myCamera;

	for (int i = 0; i < myEntities.Size(); i++)
	{
		if (myEntities[i] != myPlayer)
		{
			delete myEntities[i];
			myEntities[i] = nullptr;
		}
	}
	delete myEMP;
	delete mySkySphere;
	delete myEntityFactory;
	delete myWeaponFactory;
	delete myBulletManager;
	delete myCollisionManager;
	delete myMissionManager;
	delete myEventManager;
	delete myConversationManager;
	delete myScene;
	myEMP = nullptr;
	mySkySphere = nullptr;
	myScene = nullptr;

	delete myRenderer;
	myRenderer = nullptr;
	
	delete myEmitterManager;
	myEmitterManager = nullptr;

	Prism::Engine::GetInstance()->GetFileWatcher()->Clear();
}

bool Level::LogicUpdate(float aDeltaTime)
{
	myCollisionManager->CleanUp();

	if (myPlayer->GetAlive() == false || myEntityToDefend != nullptr && myEntityToDefend->GetAlive() == false)
	{
		return true;
	}

	for (int i = myEntities.Size() - 1; i >= 0; --i)
	{
		if (myEntities[i]->GetAlive() == false)
		{
			myEntities.DeleteCyclicAtIndex(i);
			continue;
		}

		myEntities[i]->Update(aDeltaTime);

		if (myEntities[i]->GetType() == eEntityType::ENEMY)
		{
			myPlayer->SendNote<GUINote>(GUINote(myEntities[i], eGUINoteType::ENEMY));
		}
	}

	if (myEMPActivated == true)
	{
		myEMPTimer -= aDeltaTime;
		myEMP->GetComponent<GraphicsComponent>()->SetScale({ myEMPScale, myEMPScale, myEMPScale });
		//myEMP->myOrientation = CU::Matrix44<float>::CreateRotateAroundZ(aDeltaTime) * myEMP->myOrientation;
		myEMPScale += 10000 * aDeltaTime;
		if (myEMPTimer <= 0.f)
		{
			myEMPScale = 1.f;
			myEMP->GetComponent<GraphicsComponent>()->SetScale({ myEMPScale, myEMPScale, myEMPScale });
			myEMPActivated = false;
		}
	}

	myEmitterManager->UpdateEmitters(aDeltaTime,myWorldMatrix);


	////streak debug only, please remove later
	//static float totalTime = 0;
	//totalTime += aDeltaTime;

	//float radius = 34.f;
	//float distance = 100.f;

	//myStreakEntity->myOrientation.SetPos(CU::Vector3<float>(sinf(totalTime) * radius, cosf(totalTime) * radius, distance));

	//myStreakEntity->Update(aDeltaTime);
	////streak debug only END
	//myStreakEntity->GetComponent<ParticleEmitterComponent>()->Update(aDeltaTime);


	//mySkySphereOrientation.SetPos(myPlayer->myOrientation.GetPos());
	myPlayer->GetComponent<InputComponent>()->SetSkyPosition();
	UpdateDebug();

	myCollisionManager->Update();
	myBulletManager->Update(aDeltaTime);
	myMissionManager->Update(aDeltaTime);
	myEventManager->Update();
	myCamera->Update(aDeltaTime);
	return myComplete;
}

void Level::Render()
{
	
	if (myUsePostProcessing == true)
	{
		myRenderer->BeginScene();
		Prism::Engine::GetInstance()->DisableZBuffer();
		mySkySphere->Render(*myCamera);
		Prism::Engine::GetInstance()->EnableZBuffer();
		myRenderer->EndScene(Prism::ePostProcessing::NONE);

		myRenderer->BeginScene();
		myScene->Render(myBulletManager->GetInstances());


		

		myRenderer->EndScene(Prism::ePostProcessing::BLOOM);
		myRenderer->FinalRender();
	

		if (myEMPActivated == true)
		{
			//Prism::Engine::GetInstance()->DisableZBuffer();
			myEMP->GetComponent<GraphicsComponent>()->ApplyExtraTexture(myRenderer->GetFinalTexture());
			myEMP->GetComponent<GraphicsComponent>()->GetInstance()->Render(*myCamera);
			//Prism::Engine::GetInstance()->EnableZBuffer();
		}

		myEmitterManager->RenderEmitters(myCamera);
		myPlayer->GetComponent<GUIComponent>()->Render(Prism::Engine::GetInstance()->GetWindowSize(), myInputWrapper->GetMousePosition());
	}
	else
	{
		Prism::Engine::GetInstance()->DisableZBuffer();
		mySkySphere->Render(*myCamera);
		Prism::Engine::GetInstance()->EnableZBuffer();

		myScene->Render(myBulletManager->GetInstances());

		if (myEMPActivated == true)
		{
			//Prism::Engine::GetInstance()->DisableZBuffer();
			myEMP->GetComponent<GraphicsComponent>()->GetInstance()->Render(*myCamera);
			//Prism::Engine::GetInstance()->EnableZBuffer();
		}

		myEmitterManager->RenderEmitters(myCamera);

		//debug only
		//myStreakEntity->Update(1.f/30.f);
		//myStreakEntity->GetComponent<ParticleEmitterComponent>()->Render();
		//myStreakEntity->GetComponent<StreakEmitterComponent>()->Render();

		myPlayer->GetComponent<GUIComponent>()->Render(Prism::Engine::GetInstance()->GetWindowSize(), myInputWrapper->GetMousePosition());
	}
	

#ifndef RELEASE_BUILD
	Prism::Engine::GetInstance()->PrintText(static_cast<float>(myPlayer->myOrientation.GetPos().x), CU::Vector2<float>(0, 0), Prism::eTextType::DEBUG_TEXT);
	Prism::Engine::GetInstance()->PrintText(static_cast<float>(myPlayer->myOrientation.GetPos().y), CU::Vector2<float>(0, -30), Prism::eTextType::DEBUG_TEXT);
	Prism::Engine::GetInstance()->PrintText(static_cast<float>(myPlayer->myOrientation.GetPos().z), CU::Vector2<float>(0, -60), Prism::eTextType::DEBUG_TEXT);

	Prism::Engine::GetInstance()->PrintText(std::to_string(myPlayer->GetComponent<HealthComponent>()->GetHealth()), { 0, -100.f }, Prism::eTextType::DEBUG_TEXT);
	Prism::Engine::GetInstance()->PrintText(std::to_string(myPlayer->GetComponent<ShieldComponent>()->GetCurrentShieldStrength()), { 0, -120.f }, Prism::eTextType::DEBUG_TEXT);

	Prism::Engine::GetInstance()->PrintText(myPlayer->GetComponent<PhysicsComponent>()->GetVelocity().x, { 0, -140.f }, Prism::eTextType::DEBUG_TEXT);
	Prism::Engine::GetInstance()->PrintText(myPlayer->GetComponent<PhysicsComponent>()->GetVelocity().y, { 0, -160.f }, Prism::eTextType::DEBUG_TEXT);
	Prism::Engine::GetInstance()->PrintText(myPlayer->GetComponent<PhysicsComponent>()->GetVelocity().z, { 0, -180.f }, Prism::eTextType::DEBUG_TEXT);
#endif
}

void Level::OnResize(int aWidth, int aHeight)
{
	myCamera->OnResize(aWidth, aHeight);
}

Entity* Level::AddTrigger(XMLReader& aReader, tinyxml2::XMLElement* aElement)
{
	Entity* newEntity = new Entity(eEntityType::TRIGGER, *myScene, Prism::eOctreeType::NOT_IN_OCTREE);
	float entityRadius;
	aReader.ForceReadAttribute(aElement, "radius", entityRadius);
	myEntityFactory->CopyEntity(newEntity, "trigger");

	newEntity->GetComponent<CollisionComponent>()->SetCollisionRadius(entityRadius);

	tinyxml2::XMLElement* triggerElement = aReader.ForceFindFirstChild(aElement, "position");
	CU::Vector3<float> triggerPosition;
	aReader.ForceReadAttribute(triggerElement, "X", triggerPosition.x);
	aReader.ForceReadAttribute(triggerElement, "Y", triggerPosition.y);
	aReader.ForceReadAttribute(triggerElement, "Z", triggerPosition.z);
	newEntity->myOriginalOrientation.SetPos(triggerPosition*10.f);
	newEntity->myOrientation = newEntity->myOriginalOrientation;

	myEntities.Add(newEntity);
	myCollisionManager->Add(myEntities.GetLast()->GetComponent<CollisionComponent>(), eEntityType::TRIGGER);

	return myEntities.GetLast();
}

Entity* Level::GetEntityWithName(const std::string& aName)
{
	for (int i = 0; i < myEntities.Size(); ++i)
	{
		if (CU::ToLower(myEntities[i]->GetName()) == CU::ToLower(aName))
		{
			return myEntities[i];
		}
	}
	return nullptr;
}

int Level::GetEnemiesAlive() const
{
	return myCollisionManager->GetEnemiesAlive();
}

Entity* Level::GetPlayer()
{
	return myPlayer;
}

void Level::ReceiveMessage(const SpawnEnemyMessage& aMessage)
{
	Entity* newEntity = new Entity(eEntityType::ENEMY, *myScene, Prism::eOctreeType::DYNAMIC);
	myEntityFactory->CopyEntity(newEntity, aMessage.myType);
	if (aMessage.myPowerUpName != "")
	{
		newEntity->SetPowerUpName(aMessage.myPowerUpName);
	}

	newEntity->myOrientation.SetPos(aMessage.myPosition);
	
	newEntity->myOrientation = CU::GetOrientation(newEntity->myOrientation, aMessage.myRotation);

	myCollisionManager->Add(newEntity->GetComponent<CollisionComponent>(), eEntityType::ENEMY);

	if (myEntityToDefend != nullptr)
	{
		newEntity->GetComponent<AIComponent>()->SetEntityToFollow(myEntityToDefend, myPlayer);
	}
	else
	{
		newEntity->GetComponent<AIComponent>()->SetEntityToFollow(myPlayer, myPlayer);
	}

	myEntities.Add(newEntity);

	myScene->AddInstance(newEntity->GetComponent<GraphicsComponent>()->GetInstance());
}

void Level::ReceiveMessage(const PowerUpMessage& aMessage)
{
	if (aMessage.GetPowerupType() == ePowerUpType::WEAPON_UPGRADE)
	{
		myPlayer->GetComponent<ShootingComponent>()->UpgradeWeapon(myWeaponFactory->GetWeapon(aMessage.GetUprgade()), aMessage.GetUpgradeID());
		if (aMessage.GetUpgradeID() < 2)
		{
			myPlayer->GetComponent<ShootingComponent>()->SetCurrentWeaponID(aMessage.GetUpgradeID()); // Autochanges weapon on upgrade
		}
	}
}

void Level::ReceiveMessage(const DefendMessage& aMessage)
{
	if (aMessage.myType == DefendMessage::eType::ENTITY)
	{
		myEntityToDefend = aMessage.myEntity;
	}
	else if (aMessage.myType == DefendMessage::eType::COMPLETE)
	{
		myEntityToDefend = nullptr;
	}
}

void Level::ReceiveMessage(const SpawnPowerUpMessage& aMessage)
{
	Entity* newEntity = new Entity(eEntityType::POWERUP, *myScene, Prism::eOctreeType::STATIC);

	newEntity->myOriginalOrientation.SetPos(aMessage.myOrientation.GetPos());
	newEntity->myOrientation = aMessage.myOrientation;

	myEntityFactory->CopyEntity(newEntity, aMessage.myPowerUpName);
	newEntity->GetComponent<PowerUpComponent>()->SetPlayer(myPlayer);
	myCollisionManager->Add(newEntity->GetComponent<CollisionComponent>(), eEntityType::POWERUP);
	myEntities.Add(newEntity);
	myScene->AddInstance(newEntity->GetComponent<GraphicsComponent>()->GetInstance());

	if (newEntity->GetComponent<ParticleEmitterComponent>() != nullptr)
	{
		myEmitterManager->AddEmitter(newEntity->GetComponent<ParticleEmitterComponent>());
	}
}

void Level::ReceiveMessage(const EMPMessage& aMessage)
{
	myEMP->myOrientation.SetPos(myPlayer->myOrientation.GetPos());
	myEMPTimer = aMessage.myEMPTime;
	myEMPScale = 1.f;
	myEMPActivated = true;
}

void Level::CompleteLevel()
{
	PostMaster::GetInstance()->SendMessage(GameStateMessage(eGameState::COMPLETE_LEVEL));
}

void Level::UpdateDebug()
{
	if (myInputWrapper->KeyDown(DIK_N) == true)
	{
		myPlayer->GetComponent<HealthComponent>()->RemoveHealth(10);
	}
	if (myInputWrapper->KeyDown(DIK_M) == true)
	{
		myPlayer->GetComponent<HealthComponent>()->RemoveHealth(myPlayer->GetComponent<HealthComponent>()->GetHealth());
	}
	if (myInputWrapper->KeyDown(DIK_V) == true)
	{
		myPlayer->GetComponent<HealthComponent>()->SetInvulnerability();
	}
	if (myInputWrapper->KeyDown(DIK_B) == true)
	{
		CompleteLevel();
	}
	if (myInputWrapper->KeyDown(DIK_P))
	{
		Prism::Engine::GetInstance()->ToggleWireframe();
	}
	if (myInputWrapper->KeyDown(DIK_7))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("Mute", 0);
	}
	if (myInputWrapper->KeyDown(DIK_8))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("UnMute", 0);
	}
	if (myInputWrapper->KeyDown(DIK_9))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("LowerVolume", 0);
	}
	if (myInputWrapper->KeyDown(DIK_0))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("IncreaseVolume", 0);
	}

	if (myInputWrapper->KeyDown(DIK_T))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("MuteMusic", 0);
	}
	if (myInputWrapper->KeyDown(DIK_Y))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("UnMuteMusic", 0);
	}
	if (myInputWrapper->KeyDown(DIK_U))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("LowerMusic", 0);
	}
	if (myInputWrapper->KeyDown(DIK_I))
	{
		Prism::Audio::AudioInterface::GetInstance()->PostEvent("IncreaseMusic", 0);
	}
	if (myInputWrapper->KeyDown(DIK_F7))
	{
		myUsePostProcessing = !myUsePostProcessing;
	}
}