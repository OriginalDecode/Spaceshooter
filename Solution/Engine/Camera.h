#pragma once
#include <Matrix44.h>

class Camera
{
public:
	Camera();
	~Camera();

	void OnResize(const int aWidth, const int aHeight);

	CU::Matrix44<float>& GetOrientation();
	CU::Vector3<float>& GetPosition();
	CU::Matrix44<float>& GetProjection();
	CU::Matrix44<float>& GetOrthogonal();

	void SetOrientation(const CU::Matrix44<float>& aOrientation);
	void SetPosition(const CU::Vector3<float>& aPosition);

	void RotateX(const float aDegrees);
	void RotateY(const float aDegrees);
	void RotateZ(const float aDegrees);

	void MoveForward(const float aDistance);
	void MoveRight(const float aDistance);

private:
	CU::Matrix44<float> myOrientation;
	CU::Matrix44<float> myProjectionMatrix;
	CU::Matrix44<float> myOrthogonalMatrix;
	CU::Vector3<float> myPosition;
};

