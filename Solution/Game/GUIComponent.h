#pragma once
#include "Component.h"
#include <GrowingArray.h>
#include <Vector.h>


namespace Prism
{
	class Model2D;
	class Camera;
}

class GUIComponent : public Component
{
public:
	GUIComponent(Entity& aEntity);
	~GUIComponent();

	void Update(float aDeltaTime) override;
	void CalculateAndRender(const CU::Vector3<float>& aPosition, Prism::Model2D* aCurrentModel
		, Prism::Model2D* aArrowModel, Prism::Model2D* aMarkerModel, const CU::Vector2<int> aWindowSize, bool aShowDist);
	void Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos);

	void SetCamera(Prism::Camera* aCamera);

	static int GetID();
	void ReceiveNote(const MissionNote& aMessage) override;
	void ReceiveNote(const GUINote& aNote) override;
	
private:
	Prism::Model2D* myReticle;
	Prism::Model2D* myCurrentWaypoint;
	Prism::Model2D* myEnemyArrow;
	Prism::Model2D* myEnemyMarker;
	Prism::Model2D* myWaypointMarker;
	Prism::Model2D* myWaypointArrow;
	Prism::Model2D* myCrosshair;
	Prism::Model2D* mySteeringTarget;
	CU::Vector2<float> mySteeringTargetPosition;
	CU::Vector3<float> myWaypointPosition;

	CU::GrowingArray<CU::Vector3<float>> myEnemiesPosition;
	Prism::Model2D* myEnemiesCursor;

	Prism::Model2D* myPowerUpArrow;
	Prism::Model2D* myPowerUpMarker;
	Prism::Model2D* myPowerUpsCursor;
	CU::GrowingArray<CU::Vector3<float>> myPowerUpPositions;

	Prism::Camera* myCamera;

	bool myWaypointActive;
};

inline int GUIComponent::GetID()
{
	return 6;
}

inline void GUIComponent::SetCamera(Prism::Camera* aCamera)
{
	myCamera = aCamera;
}