#pragma once

#include "Subscriber.h"

namespace Prism
{
	class Camera;
	class DirectionalLight;
	class Model;
	class PointLight;
	class Scene;
	class SpotLight;
	class DebugText;
	class Instance;
	class Renderer;
};

namespace CU
{
	class InputWrapper;
}

class BulletManager;
class CollisionManager;
class ConversationManager;
class DefendMessage;
class EMPMessage;
class ExplosionMessage;
class Entity;
class EntityFactory;
class EmitterManager;
class EventManager;
class MissionManager;
class WeaponFactory;
class XMLReader;

namespace tinyxml2
{
	class XMLElement;
}

class Level : public Subscriber
{
	friend class LevelFactory;

public:
	Level(CU::InputWrapper* aInputWrapper);
	~Level();

	bool LogicUpdate(float aDeltaTime);
	void Render();

	void OnResize(int aWidth, int aHeigth);

	void RemoveEntity(Entity* aEntity);

	Entity* AddTrigger(XMLReader& aReader, tinyxml2::XMLElement* aElement);
	void CompleteLevel();

	int GetEnemiesAlive() const;

	Entity* GetPlayer();

	void ReceiveMessage(const SpawnEnemyMessage& aMessage) override;
	void ReceiveMessage(const PowerUpMessage& aMessage) override;
	void ReceiveMessage(const DefendMessage& aMessage) override;
	void ReceiveMessage(const SpawnPowerUpMessage& aMessage) override;
	void ReceiveMessage(const EMPMessage& aMessage) override;

	const CU::Vector2<float>& GetScreenCenterPosition();

private:
	Level& operator=(Level&) = delete;
	void ReadXML(const std::string& aFile);
	Entity* GetEntityWithName(const std::string& aName);
	void UpdateDebug();

	Prism::Instance* mySkySphere;
	CU::Matrix44<float> mySkySphereOrientation;
	float mySkySphereCullingRadius;
	Prism::Scene* myScene;
	Prism::Renderer* myRenderer;
	Prism::Camera* myCamera;
	CU::Matrix44<float> myWorldMatrix;

	CU::GrowingArray<Entity*> myEntities;
	CU::GrowingArray<Entity*> myDeadEntities;

	Entity* myPlayer;
	Entity* myEntityToDefend;

	CU::InputWrapper* myInputWrapper;

	EntityFactory* myEntityFactory;
	WeaponFactory* myWeaponFactory;

	BulletManager* myBulletManager;
	CollisionManager* myCollisionManager;
	ConversationManager* myConversationManager;

	MissionManager* myMissionManager;
	EventManager* myEventManager;

	EmitterManager* myEmitterManager;

	bool myUsePostProcessing;
	bool myComplete;

	Entity* myStreakEntity;
	Entity* myEMP;
	float myEMPScale;
	float myEMPTimer;
	bool myEMPActivated;

};

inline void Level::RemoveEntity(Entity* aEntity)
{
	myEntities.RemoveCyclic(aEntity);
}