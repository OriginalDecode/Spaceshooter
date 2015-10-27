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
}

class GUIComponent : public Component, public Subscriber
{
public:
	GUIComponent(Entity& aEntity);
	~GUIComponent();

	void Init(float aMaxDistanceToEnemies);

	void Update(float aDeltaTime) override;
	void CalculateAndRender(const CU::Vector3<float>& aPosition, Prism::Sprite* aCurrentModel
		, Prism::Sprite* aArrowModel, Prism::Sprite* aMarkerModel, const CU::Vector2<int> aWindowSize, bool aShowDist);

	void Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos);

	void SetCamera(Prism::Camera* aCamera);
	Prism::Camera* GetCamera();

	static eComponentType GetType();
	void ReceiveNote(const MissionNote& aNote) override;
	void ReceiveNote(const GUINote& aNote) override;
	void ReceiveNote(const HealthNote& aNote) override;
	void ReceiveNote(const ShieldNote& aNote) override;


	void ReceiveMessage(const ConversationMessage& aMessage) override;
	void ReceiveMessage(const DefendMessage& aMessage) override;
	void ReceiveMessage(const ResizeMessage& aMessage) override;
	void ReceiveMessage(const BulletCollisionToGUIMessage& aMessage) override;
	void ReceiveMessage(const PowerUpMessage& aMessage) override;

	void ReadXML();

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
	
	float myHitMarkerTimer;
	float myDamageIndicatorTimer;

	CU::Vector2<float> mySteeringTargetPosition;
	CU::Vector3<float> myWaypointPosition;

	CU::GrowingArray<Entity*> myEnemies;

	Entity* myClosestEnemy;
	CU::Vector2<float> myClosestScreenPos;
	float myClosestEnemyLength;
	bool myHasHomingWeapon;

	Prism::Sprite* myPowerUpArrow;
	Prism::Sprite* myPowerUpMarker;
	CU::GrowingArray<CU::Vector3<float>> myPowerUpPositions;

	Prism::Camera* myCamera;
	float myMaxDistanceToEnemies;
	bool myWaypointActive;

	int myHealthBarCount;
	int myShieldBarCount;

	Prism::Sprite* myHealthBar;
	Prism::Sprite* myShieldBar;

	Prism::Sprite* myHealthBarGlow;
	Prism::Sprite* myShieldBarGlow;

	CU::Vector2<float> myHealthBarRenderPosition;
	CU::Vector2<float> myOriginalHealthBarRenderPosition;

	CU::Vector2<float> myShieldBarRenderPosition;
	CU::Vector2<float> myOriginalShieldBarRenderPosition;

	float myBarSize;
	float myOriginalBarSize;


	std::string myConversation;

	Entity* myEnemiesTarget;

	bool myShowMessage;
	std::string myMessage; 
	float myMessageTime;
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