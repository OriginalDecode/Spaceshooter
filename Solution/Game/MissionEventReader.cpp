#include "stdafx.h"
#include "MissionEventReader.h"
#include <string>
#include <XMLReader.h>

void MissionEventReader::ReadEventNames(XMLReader& aNamesReader, tinyxml2::XMLElement* aNamesElement
	, CU::GrowingArray<std::string>& someStartEventsOut)
{
	for (tinyxml2::XMLElement* element = aNamesReader.FindFirstChild(aNamesElement, "event"); element != nullptr;
		element = aNamesReader.FindNextElement(element, "event"))
	{
		std::string type;
		aNamesReader.ForceReadAttribute(element, "type", type);

		if (type == "start")
		{
			std::string name;
			aNamesReader.ForceReadAttribute(element, "name", name);
			someStartEventsOut.Add(name);
		}
	}
}