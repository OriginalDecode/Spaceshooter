#pragma once
#include "PlaneVolume.h"
#include "Matrix.h"

namespace CommonUtilities
{
	namespace Intersection
	{
		class Fov90Frustum
		{
		public:
			Fov90Frustum(float aNear, float aFar);
			bool Inside(const Vector3<float>& aPos, float aRadius) const;

			CommonUtilities::PlaneVolume<float> myVolume;
			float myNear;
			float myFar;
		};
	}

	inline Intersection::Fov90Frustum::Fov90Frustum(float aNear, float aFar) :
		myVolume(CommonUtilities::GrowingArray<CommonUtilities::Plane<float>>())
	{
		myNear = aNear;
		myFar = aFar;
		float rotated45 = sqrt(2.f) / 2.f;

		CommonUtilities::Plane<float> nearPlane(CommonUtilities::Vector3<float>(0, 0, aNear)
			, CommonUtilities::Vector3<float>(0, 0, -1));
		CommonUtilities::Plane<float> farPlane(CommonUtilities::Vector3<float>(0, 0, aFar)
			, CommonUtilities::Vector3<float>(0, 0, 1));

		CommonUtilities::Plane<float> right(CommonUtilities::Vector3<float>(0, 0, 0), CommonUtilities::Vector3<float>(rotated45, 0, -rotated45));
		CommonUtilities::Plane<float> left(CommonUtilities::Vector3<float>(0, 0, 0), CommonUtilities::Vector3<float>(-rotated45, 0, -rotated45));
		CommonUtilities::Plane<float> up(CommonUtilities::Vector3<float>(0, 0, 0), CommonUtilities::Vector3<float>(0, rotated45, -rotated45));
		CommonUtilities::Plane<float> down(CommonUtilities::Vector3<float>(0, 0, 0), CommonUtilities::Vector3<float>(0, -rotated45, -rotated45));

		myVolume.myPlanes.Init(6);
		myVolume.AddPlane(nearPlane);
		myVolume.AddPlane(farPlane);
		myVolume.AddPlane(right);
		myVolume.AddPlane(left);
		myVolume.AddPlane(up);
		myVolume.AddPlane(down);
	}

	inline bool CommonUtilities::Intersection::Fov90Frustum::Inside(const Vector3<float>& aPos, float aRadius) const
	{
		if (myVolume.Inside(aPos, aRadius) == true)
		{
			return true;
		}
		return false;
	}
}