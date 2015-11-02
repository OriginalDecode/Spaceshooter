#pragma once
#include "Component.h"
#include <GrowingArray.h>
#include <string>
#include "Subscriber.h"
#include <Vector.h>

namespace Prism
{
	class Sprite;
	class Camera;
	class Instance;
}

class PowerUpGUIIcon;

class GUIComponent : public Component, public Subscriber
{
public:
	GUIComponent(Entity& aEntity);
	~GUIComponent();

	void Init(float aMaxDistanceToEnemies);

	void Update(float aDeltaTime) override;
	void CalculateAndRender(const CU::Vector3<float>& aPosition, Prism::Sprite* aCurrentModel
		, Prism::Sprite* aArrowModel, Prism::Sprite* aMarkerModel, const CU::Vector2<int> aWindowSize
		, bool aShowDist, bool aIsPowerup = false, std::string aName = "");

	void Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos);

	void SetCamera(Prism::Camera* aCamera);
	Prism::Camera* GetCamera();

	static eComponentType GetType();
	void ReceiveNote(const MissionNote& aNote) override;
	void ReceiveNote(const GUINote& aNote) override;
	void ReceiveNote(const HealthNote& aNote) override;
	void ReceiveNote(const ShieldNote& aNote) override;
	void ReceiveNote(const PowerUpNote& aNote) override;
	void ReceiveNote(const InputNote& aMessage) override;

	void ReceiveMessage(const ConversationMessage& aMessage) override;
	void ReceiveMessage(const DefendMessage& aMessage) override;
	void ReceiveMessage(const ResizeMessage& aMessage) override;
	void ReceiveMessage(const BulletCollisionToGUIMessage& aMessage) override;
	void ReceiveMessage(const PowerUpMessage& aMessage) override;
	void ReceiveMessage(const KillStructureMessage& aMessage) override;



	void Reset() override;

	Entity* GetClosestEnemy();

private:
	Prism::Sprite* myReticle;
	Prism::Sprite* myModel2DToRender;
	Prism::Sprite* myEnemyArrow;
	Prism::Sprite* myEnemyMarker;
	Prism::Sprite* myWaypointMarker;
	Prism::Sprite* myWaypointArrow;
	Prism::Sprite* myDefendMarker;
	Prism::Sprite* myDefendArrow;
	Prism::Sprite* myCrosshair;
	Prism::Sprite* mySteeringTarget;
	Prism::Sprite* myHitMarker;
	Prism::Sprite* myDamageIndicator;
	Prism::Sprite* myHomingTarget;

	Prism::Sprite* myStructureArrow;
	Prism::Sprite* myStructureMarker;
	
	float myHitMarkerTimer;
	float myDamageIndicatorTimer;

	CU::Vector2<float> mySteeringTargetPosition;
	CU::Vector3<float> myWaypointPosition;

	CU::GrowingArray<Entity*> myEnemies;

	Entity* myClosestEnemy;
	CU::Vector2<float> myClosestScreenPos;
	float myClosestEnemyLength;
	bool myHasHomingWeapon;
	float my3DClosestEnemyLength;
	bool myBattlePlayed;
	bool myBackgroundMusicPlayed;

	Prism::Sprite* myPowerUpArrow;
	Prism::Sprite* myPowerUpMarker;
	CU::GrowingArray<Entity*> myPowerUps;

	std::unordered_map<ePowerUpType, PowerUpGUIIcon*> myPowerUpSlots;

	Prism::Camera* myCamera;
	float myMaxDistanceToEnemies;
	bool myWaypointActive;

	Prism::Instance* myGUIBars[2];
	float myHealthBarRadius;
	float myShieldBarRadius;

	std::string myConversation;

	Entity* myEnemiesTarget;

	bool myShowMessage;
	std::string myMessage; 
	float myMessageTime;

	std::string myWeapon;


};

inline eComponentType GUIComponent::GetType()
{
	return eComponentType::GUI;
}

inline void GUIComponent::SetCamera(Prism::Camera* aCamera)
{
	myCamera = aCamera;
}

inline Prism::Camera* GUIComponent::GetCamera()
{
	return myCamera;
}

inline Entity* GUIComponent::GetClosestEnemy()
{
	return myClosestEnemy;
}