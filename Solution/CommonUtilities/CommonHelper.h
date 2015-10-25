#pragma once
#include <algorithm>
#include <string>
#include <DL_Debug.h>

namespace CU
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

	inline std::string GetSubString(const std::string& aStringToReadFrom, const std::string& aWordToFind, bool aReadAfterChar
		, int someCharsToSkip = 0)
	{
		std::string toReturn;
		if (aStringToReadFrom.rfind(aWordToFind) != std::string::npos)
		{
			if (aReadAfterChar == false)
			{
				return toReturn = aStringToReadFrom.substr(0, aStringToReadFrom.rfind(aWordToFind));
			}
			else if (aReadAfterChar == true)
			{
				return toReturn = aStringToReadFrom.substr(aStringToReadFrom.rfind(aWordToFind) + (someCharsToSkip - 1));
			}
		}
		//DL_DEBUG("Attempting to load : %s", aStringToReadFrom.c_str());
		//DL_ASSERT("Failed to find texture.");

		return aStringToReadFrom;
	}


	inline std::string Concatenate(const char* aFormattedString, ...)
	{
		char buffer[1024];
		va_list args;
		va_start(args, aFormattedString);
		vsprintf_s(buffer, aFormattedString, args);
		perror(buffer);
		va_end(args);

		return buffer;
	}
}