#pragma once
#include <algorithm>
#include <string>
#include "Macros.h"

namespace CommonUtilities
{
	inline std::string ToLower(const std::string& aString)
	{
		std::string data = aString;
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}

	inline std::string ToLower(const char* aString)
	{
		std::string data = aString;
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}

	inline float Clip(float aNumber, float aLower, float aUpper)
	{
		return MAX(aLower, MIN(aNumber, aUpper));
	}
}
namespace CU = CommonUtilities;