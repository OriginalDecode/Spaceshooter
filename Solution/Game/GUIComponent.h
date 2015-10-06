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
	void Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos);

	void SetCamera(Prism::Camera* aCamera);

	static int GetID();
	void ReceiveNote(const SteeringTargetNote& aMessage) override;
	void ReceiveNote(const WaypointNote& aMessage) override;
	void ReceiveNote(const EnemiesTargetNote& aMessage) override;
	
private:
	Prism::Model2D* myCurrentWaypointSprite;
	Prism::Model2D* myArrow;
	Prism::Model2D* myWaypoint;
	Prism::Model2D* myCrosshair;
	Prism::Model2D* mySteeringTarget;
	CU::Vector2<float> mySteeringTargetPosition;
	CU::Vector3<float> myWaypointPosition;

	CU::GrowingArray<CU::Vector3<float>> myEnemiesPosition;
	Prism::Model2D* myEnemiesCursor;

	Prism::Camera* myCamera;
};

inline int GUIComponent::GetID()
{
	return 6;
}

inline void GUIComponent::SetCamera(Prism::Camera* aCamera)
{
	myCamera = aCamera;
}