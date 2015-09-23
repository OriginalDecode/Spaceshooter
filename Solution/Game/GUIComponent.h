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
	GUIComponent();
	~GUIComponent();

	void Update(float aDeltaTime);

	void SetPositions(const CU::Vector2<float> aScreenCenter, const CU::Vector2<float> aMousePos);
	void SetCamera(Prism::Camera* aCamera);

	static int GetID();

private:

	Prism::Model2D* myCrosshair;
	Prism::Model2D* myMousePointer;

	CU::Vector2<float> myCenter;
	CU::Vector2<float> myMousePos;

	Prism::Camera* myCamera;
};

inline int GUIComponent::GetID()
{
	return 6;
}

inline void GUIComponent::SetPositions(const CU::Vector2<float> aScreenCenter, const CU::Vector2<float> aMousePos)
{
	myCenter = aScreenCenter;
	myMousePos = aMousePos;
}

inline void GUIComponent::SetCamera(Prism::Camera* aCamera)
{
	myCamera = aCamera;
}