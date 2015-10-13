#pragma once
#include <GrowingArray.h>
#include <string>

namespace tinyxml2
{
	class XMLElement;
}

class XMLReader;

class EventFactory
{
public:
	static void ReadEventNames(XMLReader& aReader, tinyxml2::XMLElement* aElement
		, CU::GrowingArray<std::string>& someStartEventNamessOut);
};

