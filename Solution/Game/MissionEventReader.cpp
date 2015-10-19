#include "stdafx.h"
#include "MissionEventReader.h"
#include <string>
#include <XMLReader.h>

void MissionEventReader::ReadEventNames(XMLReader& aNamesReader, tinyxml2::XMLElement* aNamesElement
	, CU::GrowingArray<std::string>& someStartEventsOut, CU::GrowingArray<std::string>& someEndEventsOut)
{
	for (tinyxml2::XMLElement* element = aNamesReader.FindFirstChild(aNamesElement, "event"); element != nullptr;
		element = aNamesReader.FindNextElement(element, "event"))
	{
		std::string type;
		aNamesReader.ForceReadAttribute(element, "type", type);
		
		std::string name;
		aNamesReader.ForceReadAttribute(element, "name", name);
		DL_ASSERT_EXP(name != "", "Error, event type " + type + "has no name.");

		if (type == "start")
		{
			someStartEventsOut.Add(name);
		}
		else if (type == "end")
		{
			someEndEventsOut.Add(name);
		}
		else
		{
			DL_ASSERT("Invalid event type: " + type);
		}
	}
}