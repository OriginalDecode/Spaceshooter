#pragma once
#include "Component.h"
#include <GrowingArray.h>
#include <string>
#include "Subscriber.h"
#include <Vector.h>

namespace Prism
{
	class Model2D;
	class Camera;
}

class GUIComponent : public Component, public Subscriber
{
public:
	GUIComponent(Entity& aEntity);
	~GUIComponent();

	void Init(float aMaxDistanceToEnemies);

	void Update(float aDeltaTime) override;
	void CalculateAndRender(const CU::Vector3<float>& aPosition, Prism::Model2D* aCurrentModel
		, Prism::Model2D* aArrowModel, Prism::Model2D* aMarkerModel, const CU::Vector2<int> aWindowSize, bool aShowDist);
	void CalculateAndRender(const CU::Vector2<float>& aPosition
		, Prism::Model2D* aCurrentModel, const CU::Vector2<int> aWindowSize);

	void Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos);

	void SetCamera(Prism::Camera* aCamera);

	static eComponentType GetType();
	void ReceiveNote(const MissionNote& aNote) override;
	void ReceiveNote(const GUINote& aNote) override;
	void ReceiveNote(const HealthNote& aNote) override;
	void ReceiveNote(const ShieldNote& aNote) override;


	void ReceiveMessage(const ConversationMessage& aMessage) override;
	void ReceiveMessage(const DefendMessage& aMessage) override;
	void ReceiveMessage(const ResizeMessage& aMessage) override;

	void ReadXML();

private:
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


	CU::Vector2<float> mySteeringTargetPosition;
	CU::Vector3<float> myWaypointPosition;

	CU::GrowingArray<CU::Vector3<float>> myEnemiesPosition;

	Prism::Model2D* myPowerUpArrow;
	Prism::Model2D* myPowerUpMarker;
	CU::GrowingArray<CU::Vector3<float>> myPowerUpPositions;

	Prism::Camera* myCamera;
	float myMaxDistanceToEnemies;
	bool myWaypointActive;


	int myHealthBarCount;
	int myShieldBarCount;

	Prism::Model2D* myHealthBar;
	Prism::Model2D* myShieldBar;

	Prism::Model2D* myHealthBarGlow;
	Prism::Model2D* myShieldBarGlow;


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