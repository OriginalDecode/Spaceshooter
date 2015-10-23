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
	
	void RenderHomingTarget(const CU::Vector2<int> aWindowSize);

	void SetCamera(Prism::Camera* aCamera);

	static eComponentType GetType();
	void ReceiveNote(const MissionNote& aNote) override;
	void ReceiveNote(const GUINote& aNote) override;
	void ReceiveNote(const HealthNote& aNote) override;
	void ReceiveNote(const ShieldNote& aNote) override;


	void ReceiveMessage(const ConversationMessage& aMessage) override;
	void ReceiveMessage(const DefendMessage& aMessage) override;
	void ReceiveMessage(const ResizeMessage& aMessage) override;
	void ReceiveMessage(const BulletCollisionToGUIMessage& aMessage) override;

	void ReadXML();

	void Reset() override;

	Entity* GetClosestEnemy();

private:
<<<<<<< HEAD
	Prism::Model2D* myReticle;
	Prism::Model2D* myModel2DToRender;
	Prism::Model2D* myEnemyArrow;
	Prism::Model2D* myEnemyMarker;
	Prism::Model2D* myWaypointMarker;
	Prism::Model2D* myWaypointArrow;
	Prism::Model2D* myDefendMarker;
	Prism::Model2D* myDefendArrow;
	Prism::Model2D* myCrosshair;
	Prism::Model2D* mySteeringTarget;
	Prism::Model2D* myHitMarker;
	Prism::Model2D* myDamageIndicator;
	Prism::Model2D* myHomingTarget;
=======
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
>>>>>>> 65621597537da5b9c530ff65ff0e070e4839f963
	float myHitMarkerTimer;
	float myDamageIndicatorTimer;


	CU::Vector2<float> mySteeringTargetPosition;
	CU::Vector3<float> myWaypointPosition;

	CU::GrowingArray<Entity*> myEnemies;
	Entity* myClosestEnemy;
	CU::Vector2<float> myClosestScreenPos;
	float myClosestEnemyLength;

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
};

inline eComponentType GUIComponent::GetType()
{
	return eComponentType::GUI;
}

inline void GUIComponent::SetCamera(Prism::Camera* aCamera)
{
	myCamera = aCamera;
}

inline Entity* GUIComponent::GetClosestEnemy()
{
	return myClosestEnemy;
}