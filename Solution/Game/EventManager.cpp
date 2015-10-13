#include "stdafx.h"
#include "EventManager.h"
#include <XMLReader.h>

EventManager::EventManager(const std::string& aXmlPath)
{
	XMLReader reader;
	reader.OpenDocument(aXmlPath);
	tinyxml2::XMLElement* element = reader.ForceFindFirstChild("root");
	
	for (element = reader.FindFirstChild(element, "event"); element != nullptr;
		element = reader.FindNextElement(element, "event"))
	{
		std::string name;
		reader.ForceReadAttribute(element, "name", name);

		for (element = reader.FindFirstChild(element, "spawnEnemy"); element != nullptr;
			element = reader.FindNextElement(element, "spawnEnemy"))
		{
			// move to spawnEnemy-class, then read pos, rot, scale.
			std::string type;
			reader.ForceReadAttribute(element, "type", type);

		}
	}
	reader.CloseDocument();
}


EventManager::~EventManager()
{
	for (auto it = myEvents.begin(); it != myEvents.end(); ++it)
	{
		delete it->second;
	}
}
