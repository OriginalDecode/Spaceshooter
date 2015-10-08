#pragma once
#include "Vector.h"

namespace CommonUtilities
{
	namespace Intersection
	{
		struct AABB
		{
			CommonUtilities::Vector3<float> myCenterPos;
			CommonUtilities::Vector3<float> myExtents;
			CommonUtilities::Vector3<float> myMinPos;
			CommonUtilities::Vector3<float> myMaxPos;
		};
	}
}