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