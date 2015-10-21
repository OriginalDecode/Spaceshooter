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
	class Text;
	class Instance;
	class EmitterInstance;
};

namespace CommonUtilities
{
	class InputWrapper;
}

class BulletManager;
class CollisionManager;
class ConversationManager;
class DefendMessage;
class Entity;
class EntityFactory;
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
	Level(const std::string& aFileName, CU::InputWrapper* aInputWrapper);
	Level(CU::InputWrapper* aInputWrapper);
	~Level();

	void SetSkySphere(const std::string& aModelFilePath, const std::string& aEffectFileName);

	bool LogicUpdate(float aDeltaTime);
	void Render();

	void OnResize(int aWidth, int aHeigth);

	void RemoveEntity(Entity* aEntity);

	Entity* AddTrigger(XMLReader& aReader, tinyxml2::XMLElement* aElement);
	void CompleteLevel();

	int GetEnemiesAlive() const;

	void ReceiveMessage(const SpawnEnemyMessage& aMessage) override;
	void ReceiveMessage(const PowerUpMessage& aMessage) override;
	void ReceiveMessage(const DefendMessage& aMessage) override;

	const CU::Vector2<float>& GetScreenCenterPosition();

private:
	Level& operator=(Level&) = delete;
	void ReadXML(const std::string& aFile);
	Entity* GetEntityWithName(const std::string& aName);
	void LoadPlayer();
	void UpdateDebug();

	Prism::Instance* mySkySphere;
	CU::Matrix44<float> mySkySphereOrientation;
	Prism::Scene* myScene;
	Prism::Camera* myCamera;
	Prism::EmitterInstance* myEmitter;
	CU::Matrix44<float> myWorldMatrix;

	CU::GrowingArray<Entity*> myEntities;
	CU::GrowingArray<Entity*> myDeadEntities;

	CU::GrowingArray<Prism::DirectionalLight*> myDirectionalLights;
	CU::GrowingArray<Prism::PointLight*> myPointLights;
	CU::GrowingArray<Prism::SpotLight*> mySpotLights;

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

	bool myComplete;
};

inline void Level::RemoveEntity(Entity* aEntity)
{
	myEntities.RemoveCyclic(aEntity);
}