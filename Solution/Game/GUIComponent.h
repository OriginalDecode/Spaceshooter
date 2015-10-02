#pragma once
#include "Component.h"
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

	void Render(const CU::Vector2<int> aWindowSize, const CU::Vector2<float> aMousePos);

	void SetCamera(Prism::Camera* aCamera);

	static int GetID();
	void ReceiveMessage(const SteeringTargetMessage& aMessage) override;
	void ReceiveMessage(const WaypointMessage& aMessage) override;

private:
	Prism::Model2D* myCurrentWaypointSprite;
	Prism::Model2D* myArrow;
	Prism::Model2D* myWaypoint;
	Prism::Model2D* myCrosshair;
	Prism::Model2D* mySteeringTarget;
	CU::Vector2<float> mySteeringTargetPosition;
	CU::Vector3<float> myWaypointPosition;

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