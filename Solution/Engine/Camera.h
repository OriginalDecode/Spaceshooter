#pragma once
#include <Matrix44.h>

namespace Prism
{
	class Frustum;

	class Camera
	{
	public:
		Camera(CU::Matrix44f& aPlayerMatrix);
		~Camera();

		void ReadXML(const std::string& aFileName);

		void OnResize(const int aWidth, const int aHeight);

		const CU::Matrix44<float>& GetOrientation() const;
		const CU::Matrix44<float>& GetProjection() const;
		const CU::Matrix44<float>& GetOrthogonal() const;

		void Update();

		const Frustum& GetFrustum() const;

		void SetOrientation(const CU::Matrix44<float>& aOrientation);
		void SetPosition(const CU::Vector3<float>& aPosition);

		void RotateX(const float aDegrees);
		void RotateY(const float aDegrees);
		void RotateZ(const float aDegrees);

		void MoveForward(const float aDistance);
		void MoveRight(const float aDistance);

	private:
		void operator= (const Camera&) = delete;

		CU::Matrix44<float>& myOrientation;
		CU::Matrix44<float> myProjectionMatrix;
		CU::Matrix44<float> myOrthogonalMatrix;
		float myNear;
		float myFar;
		float myFOV;
		Frustum* myFrustum;
	};

	inline const Frustum& Camera::GetFrustum() const
	{
		return *myFrustum;
	}
}