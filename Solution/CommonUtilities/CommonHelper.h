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

	inline std::string ToLower(const char* aString)
	{
		std::string data = aString;
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}

	inline float Clip(float aNumber, float aLower, float aUpper)
	{
		return fmax(aLower, fmin(aNumber, aUpper));
	}

	inline std::string GetSubString(std::string aStringToReadFrom, char aCharToFind, bool aReadAfterChar)
	{
		std::string toReturn;
		if (aReadAfterChar == false)
		{
			toReturn = aStringToReadFrom.substr(0, aStringToReadFrom.find(aCharToFind));
		}
		else if (aReadAfterChar == true)
		{
			toReturn = aStringToReadFrom.substr(aStringToReadFrom.find_first_of(aCharToFind) + 1);
		}

		return toReturn;
	}
}
namespace CU = CommonUtilities;