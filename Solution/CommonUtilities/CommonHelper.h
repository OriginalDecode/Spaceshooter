#pragma once
#include <algorithm>
#include <string>

namespace CommonUtilities
{
	inline std::string ToLower(const std::string& aString)
	{
		std::string data = aString;
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}
}
namespace CU = CommonUtilities;